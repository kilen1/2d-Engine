<?php
require_once '../../config/database.php';
require_once '../../auth/session.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$database = new Database();
$db = $database->getConnection();
$auth = new Auth($db);

if(!$auth->isLoggedIn()) {
    echo json_encode(['is_admin' => false, 'message' => 'Требуется авторизация']);
    exit;
}

if($auth->isAdmin()) {
    echo json_encode([
        'is_admin' => true,
        'user' => $auth->getUserInfo()
    ]);
} else {
    echo json_encode([
        'is_admin' => false,
        'message' => 'Доступ запрещен'
    ]);
}

$database->closeConnection();
?>