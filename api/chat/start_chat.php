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

$user_id = $_SESSION['user_id'];

try {
    // Проверяем, есть ли уже активный чат
    $query = "SELECT id FROM chats 
             WHERE user_id = :user_id AND status = 'open' 
             LIMIT 1";
    $stmt = $db->prepare($query);
    $stmt->bindParam(":user_id", $user_id);
    $stmt->execute();
    
    if($stmt->rowCount() > 0) {
        $chat = $stmt->fetch(PDO::FETCH_ASSOC);
        echo json_encode(['success' => true, 'chat_id' => $chat['id']]);
        exit;
    }
    
    // Создаем новый чат
    $query = "INSERT INTO chats (user_id, status, created_at) 
             VALUES (:user_id, 'open', NOW())";
    $stmt = $db->prepare($query);
    $stmt->bindParam(":user_id", $user_id);
    
    if($stmt->execute()) {
        $chat_id = $db->lastInsertId();
        
        // Создаем системное приветственное сообщение
        $welcome_message = "Добро пожаловать в чат поддержки! Мы ответим вам в ближайшее время.";
        $query = "INSERT INTO messages (chat_id, sender_id, message, created_at) 
                 VALUES (:chat_id, 0, :message, NOW())";
        $stmt = $db->prepare($query);
        $stmt->bindParam(":chat_id", $chat_id);
        $stmt->bindParam(":message", $welcome_message);
        $stmt->execute();
        
        echo json_encode(['success' => true, 'chat_id' => $chat_id]);
    } else {
        echo json_encode(['success' => false, 'message' => 'Ошибка создания чата']);
    }
    
} catch(PDOException $e) {
    error_log("Start chat error: " . $e->getMessage());
    echo json_encode(['success' => false, 'message' => 'Ошибка базы данных']);
}

$database->closeConnection();
?>