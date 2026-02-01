<?php
require_once '../../config/database.php';
require_once '../../auth/session.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST');
header('Access-Control-Allow-Headers: Content-Type');

$database = new Database();
$db = $database->getConnection();
$auth = new Auth($db);

if(!$auth->isLoggedIn()) {
    echo json_encode(['success' => false, 'message' => 'Требуется авторизация']);
    exit;
}

$data = json_decode(file_get_contents("php://input"));
$user_id = $_SESSION['user_id'];

if(!isset($data->message) || empty(trim($data->message))) {
    echo json_encode(['success' => false, 'message' => 'Сообщение не может быть пустым']);
    exit;
}

$message = trim($data->message);
$attached_plan_id = isset($data->attached_plan_id) ? intval($data->attached_plan_id) : null;

try {
    // Находим активный чат пользователя
    $query = "SELECT id FROM chats 
             WHERE user_id = :user_id AND status = 'open' 
             ORDER BY created_at DESC LIMIT 1";
    $stmt = $db->prepare($query);
    $stmt->bindParam(":user_id", $user_id);
    $stmt->execute();
    
    if($stmt->rowCount() == 0) {
        echo json_encode(['success' => false, 'message' => 'Нет активного чата']);
        exit;
    }
    
    $chat = $stmt->fetch(PDO::FETCH_ASSOC);
    $chat_id = $chat['id'];
    
    // Добавляем сообщение
    $query = "INSERT INTO messages (chat_id, sender_id, message, attached_plan_id, created_at) 
             VALUES (:chat_id, :sender_id, :message, :attached_plan_id, NOW())";
    $stmt = $db->prepare($query);
    $stmt->bindParam(":chat_id", $chat_id);
    $stmt->bindParam(":sender_id", $user_id);
    $stmt->bindParam(":message", $message);
    $stmt->bindParam(":attached_plan_id", $attached_plan_id);
    
    if($stmt->execute()) {
        // Обновляем время последнего сообщения в чате
        $update_query = "UPDATE chats SET last_message_at = NOW() WHERE id = :chat_id";
        $update_stmt = $db->prepare($update_query);
        $update_stmt->bindParam(":chat_id", $chat_id);
        $update_stmt->execute();
        
        // Создаем уведомление для администраторов
        if(!$auth->isAdmin()) {
            $notification_query = "INSERT INTO notifications (user_id, message, type, created_at) 
                                 SELECT id, :message, 'new_message', NOW() 
                                 FROM users WHERE role = 'admin'";
            $notification_stmt = $db->prepare($notification_query);
            $notification_stmt->bindValue(':message', "Новое сообщение в чате #{$chat_id} от пользователя ID: {$user_id}");
            $notification_stmt->execute();
        }
        
        echo json_encode([
            'success' => true,
            'chat_id' => $chat_id,
            'message_id' => $db->lastInsertId()
        ]);
    } else {
        echo json_encode(['success' => false, 'message' => 'Ошибка отправки сообщения']);
    }
    
} catch(PDOException $e) {
    error_log("Send message error: " . $e->getMessage());
    echo json_encode(['success' => false, 'message' => 'Ошибка базы данных']);
}

$database->closeConnection();
?>