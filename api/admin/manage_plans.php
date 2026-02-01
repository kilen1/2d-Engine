<?php
require_once '../../config/database.php';
require_once '../../auth/session.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST, PUT, DELETE');
header('Access-Control-Allow-Headers: Content-Type');

$database = new Database();
$db = $database->getConnection();
$auth = new Auth($db);

if(!$auth->isLoggedIn() || !$auth->isAdmin()) {
    echo json_encode(['success' => false, 'message' => 'Доступ запрещен']);
    exit;
}

$method = $_SERVER['REQUEST_METHOD'];
$data = json_decode(file_get_contents("php://input"), true);

try {
    switch($method) {
        case 'GET':
            // Получить все тарифы
            $query = "SELECT * FROM plans ORDER BY price ASC";
            $stmt = $db->prepare($query);
            $stmt->execute();
            $plans = $stmt->fetchAll(PDO::FETCH_ASSOC);
            echo json_encode($plans);
            break;
            
        case 'POST':
            // Создать новый тариф
            if(!isset($data['name']) || !isset($data['price'])) {
                echo json_encode(['success' => false, 'message' => 'Не все обязательные поля заполнены']);
                break;
            }
            
            $query = "INSERT INTO plans (name, description, price, features, is_active, created_at) 
                     VALUES (:name, :description, :price, :features, :is_active, NOW())";
            $stmt = $db->prepare($query);
            
            $stmt->bindParam(":name", $data['name']);
            $stmt->bindParam(":description", $data['description'] ?? '');
            $stmt->bindParam(":price", $data['price']);
            $stmt->bindParam(":features", $data['features'] ?? '');
            $is_active = isset($data['is_active']) ? (int)$data['is_active'] : 1;
            $stmt->bindParam(":is_active", $is_active);
            
            if($stmt->execute()) {
                $plan_id = $db->lastInsertId();
                echo json_encode([
                    'success' => true,
                    'message' => 'Тариф создан',
                    'plan_id' => $plan_id
                ]);
            } else {
                echo json_encode(['success' => false, 'message' => 'Ошибка создания тарифа']);
            }
            break;
            
        case 'PUT':
            // Обновить тариф
            if(!isset($data['id'])) {
                echo json_encode(['success' => false, 'message' => 'ID тарифа не указан']);
                break;
            }
            
            $update_fields = [];
            $params = [':id' => $data['id']];
            
            if(isset($data['name'])) {
                $update_fields[] = "name = :name";
                $params[':name'] = $data['name'];
            }
            if(isset($data['description'])) {
                $update_fields[] = "description = :description";
                $params[':description'] = $data['description'];
            }
            if(isset($data['price'])) {
                $update_fields[] = "price = :price";
                $params[':price'] = $data['price'];
            }
            if(isset($data['features'])) {
                $update_fields[] = "features = :features";
                $params[':features'] = $data['features'];
            }
            if(isset($data['is_active'])) {
                $update_fields[] = "is_active = :is_active";
                $params[':is_active'] = (int)$data['is_active'];
            }
            
            if(empty($update_fields)) {
                echo json_encode(['success' => false, 'message' => 'Нет данных для обновления']);
                break;
            }
            
            $query = "UPDATE plans SET " . implode(", ", $update_fields) . " WHERE id = :id";
            $stmt = $db->prepare($query);
            
            foreach($params as $key => $value) {
                $stmt->bindValue($key, $value);
            }
            
            if($stmt->execute()) {
                echo json_encode(['success' => true, 'message' => 'Тариф обновлен']);
            } else {
                echo json_encode(['success' => false, 'message' => 'Ошибка обновления тарифа']);
            }
            break;
            
        case 'DELETE':
            // Удалить тариф
            if(!isset($_GET['id'])) {
                echo json_encode(['success' => false, 'message' => 'ID тарифа не указан']);
                break;
            }
            
            $plan_id = intval($_GET['id']);
            
            // Проверяем, нет ли заказов с этим тарифом
            $check_query = "SELECT id FROM orders WHERE plan_id = :plan_id LIMIT 1";
            $check_stmt = $db->prepare($check_query);
            $check_stmt->bindParam(":plan_id", $plan_id);
            $check_stmt->execute();
            
            if($check_stmt->rowCount() > 0) {
                echo json_encode(['success' => false, 'message' => 'Нельзя удалить тариф, с которым есть заказы']);
                break;
            }
            
            $query = "DELETE FROM plans WHERE id = :id";
            $stmt = $db->prepare($query);
            $stmt->bindParam(":id", $plan_id);
            
            if($stmt->execute()) {
                echo json_encode(['success' => true, 'message' => 'Тариф удален']);
            } else {
                echo json_encode(['success' => false, 'message' => 'Ошибка удаления тарифа']);
            }
            break;
            
        default:
            echo json_encode(['success' => false, 'message' => 'Метод не поддерживается']);
            break;
    }
    
} catch(PDOException $e) {
    error_log("Manage plans error: " . $e->getMessage());
    echo json_encode(['success' => false, 'message' => 'Ошибка базы данных']);
}

$database->closeConnection();
?>