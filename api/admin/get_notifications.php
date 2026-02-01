<?php
require_once '../../config/database.php';
require_once '../../auth/session.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$database = new Database();
$db = $database->getConnection();
$auth = new Auth($db);

if(!$auth->isLoggedIn() || !$auth->isAdmin()) {
    echo json_encode(['success' => false, 'message' => 'Доступ запрещен']);
    exit;
}

try {
    $query = "SELECT n.id, n.message, n.type, n.is_read, n.created_at,
                     u.username, u.email
              FROM notifications n
              LEFT JOIN users u ON n.user_id = u.id
              ORDER BY n.created_at DESC
              LIMIT 50";
    
    $stmt = $db->prepare($query);
    $stmt->execute();
    
    $notifications = $stmt->fetchAll(PDO::FETCH_ASSOC);
    
    echo json_encode($notifications);
    
} catch(PDOException $e) {
    error_log("Get notifications error: " . $e->getMessage());
    echo json_encode(['success' => false, 'message' => 'Ошибка базы данных']);
}

$database->closeConnection();
?>