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

if(isset($data->username) && isset($data->email) && isset($data->password)) {
    // Валидация данных
    if(strlen($data->username) < 3) {
        echo json_encode(['success' => false, 'message' => 'Имя пользователя должно быть не менее 3 символов']);
        exit;
    }
    
    if(!filter_var($data->email, FILTER_VALIDATE_EMAIL)) {
        echo json_encode(['success' => false, 'message' => 'Неверный формат email']);
        exit;
    }
    
    if(strlen($data->password) < 6) {
        echo json_encode(['success' => false, 'message' => 'Пароль должен быть не менее 6 символов']);
        exit;
    }
    
    $result = $auth->register($data->username, $data->email, $data->password);
    echo json_encode($result);
} else {
    echo json_encode([
        'success' => false,
        'message' => 'Не предоставлены все необходимые данные'
    ]);
}

$database->closeConnection();
?>