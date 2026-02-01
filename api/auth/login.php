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

$data = json_decode(file_get_contents("php://input"));

if(isset($data->username) && isset($data->password)) {
    $result = $auth->login($data->username, $data->password);
    echo json_encode($result);
} else {
    echo json_encode([
        'success' => false,
        'message' => 'Не предоставлены данные для входа'
    ]);
}

$database->closeConnection();
?>