<?php
session_start();

class Auth {
    private $conn;
    
    public function __construct($db) {
        $this->conn = $db;
    }
    
    public function register($username, $email, $password) {
        try {
            // Проверка существования пользователя
            $query = "SELECT id FROM users WHERE username = :username OR email = :email LIMIT 1";
            $stmt = $this->conn->prepare($query);
            $stmt->bindParam(":username", $username);
            $stmt->bindParam(":email", $email);
            $stmt->execute();
            
            if($stmt->rowCount() > 0) {
                return [
                    'success' => false, 
                    'message' => 'Пользователь с таким именем или email уже существует'
                ];
            }
            
            // Хеширование пароля
            $hashed_password = password_hash($password, PASSWORD_DEFAULT);
            
            // Вставка нового пользователя
            $query = "INSERT INTO users (username, email, password, created_at) 
                     VALUES (:username, :email, :password, NOW())";
            $stmt = $this->conn->prepare($query);
            $stmt->bindParam(":username", $username);
            $stmt->bindParam(":email", $email);
            $stmt->bindParam(":password", $hashed_password);
            
            if($stmt->execute()) {
                // Получаем ID нового пользователя
                $user_id = $this->conn->lastInsertId();
                
                return [
                    'success' => true, 
                    'message' => 'Регистрация успешна',
                    'user_id' => $user_id
                ];
            }
            
            return ['success' => false, 'message' => 'Ошибка регистрации'];
            
        } catch(PDOException $e) {
            error_log("Registration error: " . $e->getMessage());
            return ['success' => false, 'message' => 'Ошибка базы данных'];
        }
    }
    
    public function login($username, $password) {
        try {
            $query = "SELECT id, username, email, password, role, created_at 
                     FROM users WHERE username = :username OR email = :username 
                     LIMIT 1";
            $stmt = $this->conn->prepare($query);
            $stmt->bindParam(":username", $username);
            $stmt->execute();
            
            if($stmt->rowCount() == 1) {
                $row = $stmt->fetch(PDO::FETCH_ASSOC);
                
                if(password_verify($password, $row['password'])) {
                    // Обновляем сессию
                    $_SESSION['user_id'] = $row['id'];
                    $_SESSION['username'] = $row['username'];
                    $_SESSION['role'] = $row['role'];
                    $_SESSION['email'] = $row['email'];
                    $_SESSION['loggedin'] = true;
                    $_SESSION['login_time'] = time();
                    
                    // Обновляем время последнего входа
                    $update_query = "UPDATE users SET last_login = NOW() WHERE id = :id";
                    $update_stmt = $this->conn->prepare($update_query);
                    $update_stmt->bindParam(":id", $row['id']);
                    $update_stmt->execute();
                    
                    return [
                        'success' => true, 
                        'user' => [
                            'id' => $row['id'],
                            'username' => $row['username'],
                            'email' => $row['email'],
                            'role' => $row['role'],
                            'created_at' => $row['created_at']
                        ]
                    ];
                }
            }
            
            return ['success' => false, 'message' => 'Неверное имя пользователя или пароль'];
            
        } catch(PDOException $e) {
            error_log("Login error: " . $e->getMessage());
            return ['success' => false, 'message' => 'Ошибка базы данных'];
        }
    }
    
    public function logout() {
        // Уничтожаем все переменные сессии
        $_SESSION = array();
        
        // Удаляем куки сессии
        if (ini_get("session.use_cookies")) {
            $params = session_get_cookie_params();
            setcookie(session_name(), '', time() - 42000,
                $params["path"], $params["domain"],
                $params["secure"], $params["httponly"]
            );
        }
        
        // Уничтожаем сессию
        session_destroy();
        
        return ['success' => true, 'message' => 'Выход выполнен'];
    }
    
    public function isLoggedIn() {
        return isset($_SESSION['loggedin']) && $_SESSION['loggedin'] === true;
    }
    
    public function isAdmin() {
        return $this->isLoggedIn() && isset($_SESSION['role']) && $_SESSION['role'] === 'admin';
    }
    
    public function getUserInfo($user_id = null) {
        if(!$user_id && isset($_SESSION['user_id'])) {
            $user_id = $_SESSION['user_id'];
        }
        
        if(!$user_id) {
            return null;
        }
        
        try {
            $query = "SELECT id, username, email, role, created_at, last_login 
                     FROM users WHERE id = :id LIMIT 1";
            $stmt = $this->conn->prepare($query);
            $stmt->bindParam(":id", $user_id);
            $stmt->execute();
            
            if($stmt->rowCount() == 1) {
                return $stmt->fetch(PDO::FETCH_ASSOC);
            }
            
            return null;
            
        } catch(PDOException $e) {
            error_log("Get user info error: " . $e->getMessage());
            return null;
        }
    }
    
    public function updateUserProfile($user_id, $data) {
        try {
            $allowed_fields = ['email', 'password'];
            $updates = [];
            $params = [':id' => $user_id];
            
            if(isset($data['email'])) {
                // Проверяем уникальность email
                $check_query = "SELECT id FROM users WHERE email = :email AND id != :id LIMIT 1";
                $check_stmt = $this->conn->prepare($check_query);
                $check_stmt->bindParam(":email", $data['email']);
                $check_stmt->bindParam(":id", $user_id);
                $check_stmt->execute();
                
                if($check_stmt->rowCount() > 0) {
                    return ['success' => false, 'message' => 'Этот email уже используется'];
                }
                
                $updates[] = "email = :email";
                $params[':email'] = $data['email'];
            }
            
            if(isset($data['password']) && !empty($data['password'])) {
                $hashed_password = password_hash($data['password'], PASSWORD_DEFAULT);
                $updates[] = "password = :password";
                $params[':password'] = $hashed_password;
            }
            
            if(empty($updates)) {
                return ['success' => false, 'message' => 'Нет данных для обновления'];
            }
            
            $query = "UPDATE users SET " . implode(", ", $updates) . " WHERE id = :id";
            $stmt = $this->conn->prepare($query);
            
            foreach($params as $key => $value) {
                $stmt->bindValue($key, $value);
            }
            
            if($stmt->execute()) {
                return ['success' => true, 'message' => 'Профиль обновлен'];
            }
            
            return ['success' => false, 'message' => 'Ошибка обновления профиля'];
            
        } catch(PDOException $e) {
            error_log("Update profile error: " . $e->getMessage());
            return ['success' => false, 'message' => 'Ошибка базы данных'];
        }
    }
    
    public function getAllUsers($limit = 100) {
        try {
            $query = "SELECT id, username, email, role, created_at, last_login 
                     FROM users ORDER BY created_at DESC LIMIT :limit";
            $stmt = $this->conn->prepare($query);
            $stmt->bindValue(':limit', (int)$limit, PDO::PARAM_INT);
            $stmt->execute();
            
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
            
        } catch(PDOException $e) {
            error_log("Get all users error: " . $e->getMessage());
            return [];
        }
    }
    
    public function deleteUser($user_id) {
        try {
            // Нельзя удалить самого себя или других администраторов
            if($user_id == $_SESSION['user_id']) {
                return ['success' => false, 'message' => 'Нельзя удалить самого себя'];
            }
            
            $query = "DELETE FROM users WHERE id = :id AND role != 'admin'";
            $stmt = $this->conn->prepare($query);
            $stmt->bindParam(":id", $user_id);
            
            if($stmt->execute()) {
                return ['success' => true, 'message' => 'Пользователь удален'];
            }
            
            return ['success' => false, 'message' => 'Ошибка удаления пользователя'];
            
        } catch(PDOException $e) {
            error_log("Delete user error: " . $e->getMessage());
            return ['success' => false, 'message' => 'Ошибка базы данных'];
        }
    }
}
?>