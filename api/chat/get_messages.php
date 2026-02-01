<?php
require_once '../../config/database.php';
require_once '../../auth/session.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$database = new Database();
$db = $database->getConnection();
$auth = new Auth($db);

if(!$auth->isLoggedIn()) {
    echo json_encode(['success' => false, 'message' => 'Требуется авторизация']);
    exit;
}

$chat_id = isset($_GET['chat_id']) ? intval($_GET['chat_id']) : 0;
$user_id = $_SESSION['user_id'];

if($chat_id <= 0) {
    echo json_encode(['success' => false, 'message' => 'Неверный ID чата']);
    exit;
}

try {
    // Проверяем права доступа к чату
    $query = "SELECT id FROM chats WHERE id = :chat_id 
             AND (user_id = :user_id OR :is_admin = 1)";
    $stmt = $db->prepare($query);
    $stmt->bindParam(":chat_id", $chat_id);
    $stmt->bindParam(":user_id", $user_id);
    $is_admin = $auth->isAdmin() ? 1 : 0;
    $stmt->bindParam(":is_admin", $is_admin);
    $stmt->execute();
    
    if($stmt->rowCount() == 0) {
        echo json_encode(['success' => false, 'message' => 'Нет доступа к этому чату']);
        exit;
    }
    
    // Получаем сообщения
    $query = "SELECT m.id, m.chat_id, m.sender_id, m.message, 
                     m.attached_plan_id, m.is_read, m.created_at,
                     p.name as plan_name, p.price as plan_price,
                     CASE WHEN m.sender_id = :user_id THEN 1 ELSE 0 END as is_current_user
              FROM messages m
              LEFT JOIN plans p ON m.attached_plan_id = p.id
              WHERE m.chat_id = :chat_id
              ORDER BY m.created_at ASC";
    
    $stmt = $db->prepare($query);
    $stmt->bindParam(":chat_id", $chat_id);
    $stmt->bindParam(":user_id", $user_id);
    $stmt->execute();
    
    $messages = $stmt->fetchAll(PDO::FETCH_ASSOC);
    
    // Помечаем сообщения как прочитанные
    if(!$auth->isAdmin()) {
        $query = "UPDATE messages SET is_read = 1 
                 WHERE chat_id = :chat_id AND sender_id != :user_id AND is_read = 0";
        $stmt = $db->prepare($query);
        $stmt->bindParam(":chat_id", $chat_id);
        $stmt->bindParam(":user_id", $user_id);
        $stmt->execute();
    }
    
    echo json_encode($messages);
    
} catch(PDOException $e) {
    error_log("Get messages error: " . $e->getMessage());
    echo json_encode(['success' => false, 'message' => 'Ошибка базы данных']);
}

$database->closeConnection();
?>