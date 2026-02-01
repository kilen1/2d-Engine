<?php
require_once '../../config/database.php';
require_once '../../auth/session.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$database = new Database();
$db = $database->getConnection();

try {
    $query = "SELECT id, name, description, price, features, is_active 
             FROM plans 
             WHERE is_active = 1 
             ORDER BY price ASC";
    
    $stmt = $db->prepare($query);
    $stmt->execute();
    
    $plans = $stmt->fetchAll(PDO::FETCH_ASSOC);
    
    echo json_encode($plans);
    
} catch(PDOException $e) {
    error_log("Get plans error: " . $e->getMessage());
    echo json_encode([
        'success' => false,
        'message' => 'Ошибка загрузки тарифов'
    ]);
}

$database->closeConnection();
?>