<?php
require_once '../../config/database.php';
require_once '../../auth/session.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$database = new Database();
$db = $database->getConnection();
$auth = new Auth($db);

if($auth->isLoggedIn()) {
    $user = $auth->getUserInfo();
    echo json_encode([
        'loggedIn' => true,
        'user' => $user
    ]);
} else {
    echo json_encode([
        'loggedIn' => false,
        'message' => 'Пользователь не авторизован'
    ]);
}

$database->closeConnection();
?>