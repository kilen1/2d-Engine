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
    $query = "SELECT o.id, o.user_id, o.plan_id, o.status, o.total_price, 
                     o.requirements, o.created_at,
                     u.username, u.email,
                     p.name as plan_name, p.price as plan_price
              FROM orders o
              JOIN users u ON o.user_id = u.id
              JOIN plans p ON o.plan_id = p.id
              ORDER BY o.created_at DESC
              LIMIT 20";
    
    $stmt = $db->prepare($query);
    $stmt->execute();
    
    $orders = $stmt->fetchAll(PDO::FETCH_ASSOC);
    
    echo json_encode($orders);
    
} catch(PDOException $e) {
    error_log("Get orders error: " . $e->getMessage());
    echo json_encode(['success' => false, 'message' => 'Ошибка базы данных']);
}

$database->closeConnection();
?>