<?php
require_once '../../config/database.php';
require_once '../../auth/session.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$database = new Database();
$db = $database->getConnection();

try {
    $query = "SELECT id, title, description, image_url, category, created_at 
             FROM portfolio 
             WHERE is_active = 1 
             ORDER BY created_at DESC";
    
    $stmt = $db->prepare($query);
    $stmt->execute();
    
    $portfolio = $stmt->fetchAll(PDO::FETCH_ASSOC);
    
    echo json_encode($portfolio);
    
} catch(PDOException $e) {
    error_log("Get portfolio error: " . $e->getMessage());
    echo json_encode([
        'success' => false,
        'message' => 'Ошибка загрузки портфолио'
    ]);
}

$database->closeConnection();
?>