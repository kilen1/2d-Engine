// Объект для хранения данных приложения
const appData = {
    users: [
        { id: 1, username: 'admin', password: 'admin123', role: 'admin', email: 'admin@example.com' },
        { id: 2, username: 'user1', password: 'user123', role: 'user', email: 'user1@example.com' }
    ],
    plans: [
        { id: 1, name: 'Базовый', description: 'Создание простого сайта-визитки', price: 15000 },
        { id: 2, name: 'Стандарт', description: 'Создание корпоративного сайта', price: 30000 },
        { id: 3, name: 'Премиум', description: 'Создание интернет-магазина', price: 50000 },
        { id: 4, name: 'SEO Продвижение', description: 'Продвижение сайта в поисковых системах', price: 20000 }
    ],
    projects: [
        { id: 1, title: 'Сайт для кафе "Уют"', image: '../images/cafe.jpg', description: 'Сайт-визитка для кафе' },
        { id: 2, title: 'Интернет-магазин одежды', image: '../images/store.jpg', description: 'Полноценный интернет-магазин' },
        { id: 3, title: 'Лендинг для курсов', image: '../images/landing.jpg', description: 'Продающий лендинг для онлайн-курсов' },
        { id: 4, title: 'Корпоративный сайт', image: '../images/corporate.jpg', description: 'Корпоративный сайт для строительной компании' }
    ],
    messages: [],
    currentUser: null,
    unreadNotifications: []
};

// Инициализация приложения
document.addEventListener('DOMContentLoaded', function() {
    loadPortfolio();
    loadPricing();
    setupEventListeners();
});

// Загрузка портфолио
function loadPortfolio() {
    const container = document.getElementById('portfolio-container');
    container.innerHTML = '';
    
    appData.projects.forEach(project => {
        const item = document.createElement('div');
        item.className = 'portfolio-item';
        item.innerHTML = `
            <img src="${project.image}" alt="${project.title}" class="portfolio-img" onerror="this.src='https://via.placeholder.com/300x250?text=Фото+работы'">
            <div class="portfolio-info">
                <h3>${project.title}</h3>
                <p>${project.description}</p>
            </div>
        `;
        container.appendChild(item);
    });
}

// Загрузка прайс-листа
function loadPricing() {
    const container = document.getElementById('pricing-container');
    container.innerHTML = '';
    
    appData.plans.forEach(plan => {
        const card = document.createElement('div');
        card.className = 'price-card';
        card.innerHTML = `
            <h3>${plan.name}</h3>
            <div class="price">${plan.price} ₽</div>
            <ul class="price-features">
                <li>${plan.description}</li>
                <li>Поддержка 30 дней</li>
                <li>Адаптивный дизайн</li>
            </ul>
            <button onclick="initiateOrderWithPlan(${plan.id})">Заказать</button>
        `;
        container.appendChild(card);
    });
    
    // Обновляем список тарифов в чате
    updatePlanSelectOptions();
}

// Обновление опций выбора тарифа в чате
function updatePlanSelectOptions() {
    const select = document.getElementById('selected-plan');
    select.innerHTML = '<option value="">Выберите тариф для прикрепления</option>';
    
    appData.plans.forEach(plan => {
        const option = document.createElement('option');
        option.value = plan.id;
        option.textContent = `${plan.name} - ${plan.price} ₽`;
        select.appendChild(option);
    });
}

// Настройка обработчиков событий
function setupEventListeners() {
    // Закрытие модальных окон
    document.querySelector('.close').onclick = function() {
        document.getElementById('account-modal').style.display = 'none';
    };
    
    document.querySelector('.close-chat').onclick = function() {
        document.getElementById('chat-modal').style.display = 'none';
    };
    
    document.querySelector('.close-admin').onclick = function() {
        document.getElementById('admin-panel').style.display = 'none';
    };
    
    // Закрытие модальных окон по клику вне их области
    window.onclick = function(event) {
        const accountModal = document.getElementById('account-modal');
        const chatModal = document.getElementById('chat-modal');
        const adminPanel = document.getElementById('admin-panel');
        
        if (event.target === accountModal) {
            accountModal.style.display = 'none';
        }
        if (event.target === chatModal) {
            chatModal.style.display = 'none';
        }
        if (event.target === adminPanel) {
            adminPanel.style.display = 'none';
        }
    };
    
    // Обработка формы добавления тарифа
    document.getElementById('pricing-form').onsubmit = function(e) {
        e.preventDefault();
        addNewPlan();
    };
}

// Показать модальное окно аккаунта
function showAccount() {
    document.getElementById('account-modal').style.display = 'block';
    // Сброс форм
    document.getElementById('login-form').style.display = 'block';
    document.getElementById('register-form').style.display = 'none';
    document.getElementById('user-dashboard').style.display = 'none';
}

// Переключение между формами входа и регистрации
function showRegister() {
    document.getElementById('login-form').style.display = 'none';
    document.getElementById('register-form').style.display = 'block';
}

function showLogin() {
    document.getElementById('register-form').style.display = 'none';
    document.getElementById('login-form').style.display = 'block';
}

// Регистрация нового пользователя
function register() {
    const username = document.getElementById('reg-username').value;
    const email = document.getElementById('reg-email').value;
    const password = document.getElementById('reg-password').value;
    
    if (!username || !email || !password) {
        alert('Пожалуйста, заполните все поля');
        return;
    }
    
    // Проверка на существование пользователя
    const existingUser = appData.users.find(user => user.username === username);
    if (existingUser) {
        alert('Пользователь с таким именем уже существует');
        return;
    }
    
    // Добавление нового пользователя
    const newUser = {
        id: appData.users.length + 1,
        username: username,
        password: password,
        role: 'user',
        email: email
    };
    
    appData.users.push(newUser);
    alert('Регистрация прошла успешно! Теперь вы можете войти.');
    showLogin();
}

// Вход в систему
function login() {
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    
    const user = appData.users.find(u => u.username === username && u.password === password);
    
    if (user) {
        appData.currentUser = user;
        
        // Обновление интерфейса
        document.getElementById('user-name').textContent = user.username;
        document.getElementById('user-status').textContent = user.role === 'admin' ? 'администратор' : 'пользователь';
        
        document.getElementById('login-form').style.display = 'none';
        document.getElementById('user-dashboard').style.display = 'block';
        
        // Если пользователь админ, показываем ссылку на админку
        if (user.role === 'admin') {
            document.getElementById('account-content').innerHTML += '<button onclick="openAdminPanel()">Админ панель</button>';
        }
    } else {
        alert('Неверное имя пользователя или пароль');
    }
}

// Выход из системы
function logout() {
    appData.currentUser = null;
    document.getElementById('login-form').style.display = 'block';
    document.getElementById('user-dashboard').style.display = 'none';
    document.getElementById('register-form').style.display = 'none';
}

// Открытие чата с администратором
function openChat() {
    if (!appData.currentUser) {
        alert('Сначала войдите в аккаунт');
        return;
    }
    
    document.getElementById('chat-modal').style.display = 'block';
    loadChatMessages();
}

// Открытие админ панели
function openAdminPanel() {
    if (!appData.currentUser || appData.currentUser.role !== 'admin') {
        alert('Только администратор может открыть эту панель');
        return;
    }
    
    document.getElementById('admin-panel').style.display = 'block';
    loadAdminNotifications();
    loadCurrentPricingList();
}

// Загрузка сообщений чата
function loadChatMessages() {
    const messagesContainer = document.getElementById('chat-messages');
    messagesContainer.innerHTML = '';
    
    const filteredMessages = appData.messages.filter(msg => 
        msg.senderId === appData.currentUser.id || 
        msg.recipientId === appData.currentUser.id ||
        appData.currentUser.role === 'admin'
    );
    
    filteredMessages.forEach(message => {
        const messageElement = document.createElement('div');
        messageElement.className = `message ${message.senderId === appData.currentUser.id ? 'user' : 'admin'}`;
        
        let messageText = message.text;
        if (message.attachedPlanId) {
            const plan = appData.plans.find(p => p.id == message.attachedPlanId);
            if (plan) {
                messageText += `<br><em>Прикрепленный тариф: ${plan.name} (${plan.price} ₽)</em>`;
            }
        }
        
        messageElement.innerHTML = messageText;
        messagesContainer.appendChild(messageElement);
    });
    
    // Прокрутка вниз
    messagesContainer.scrollTop = messagesContainer.scrollHeight;
}

// Отправка сообщения
let selectedPlanForAttachment = null;

function attachPlanToMessage() {
    const select = document.getElementById('selected-plan');
    selectedPlanForAttachment = select.value;
}

function sendMessage() {
    const messageInput = document.getElementById('message-input');
    const messageText = messageInput.value.trim();
    
    if (!messageText) {
        alert('Введите текст сообщения');
        return;
    }
    
    if (!appData.currentUser) {
        alert('Сначала войдите в аккаунт');
        return;
    }
    
    // Найти администратора
    const admin = appData.users.find(user => user.role === 'admin');
    if (!admin) {
        alert('Администратор не найден');
        return;
    }
    
    // Создать сообщение
    const newMessage = {
        id: appData.messages.length + 1,
        senderId: appData.currentUser.id,
        recipientId: admin.id,
        text: messageText,
        timestamp: new Date(),
        attachedPlanId: selectedPlanForAttachment
    };
    
    appData.messages.push(newMessage);
    
    // Очистить поле ввода
    messageInput.value = '';
    selectedPlanForAttachment = null;
    document.getElementById('selected-plan').value = '';
    
    // Обновить сообщения в чате
    loadChatMessages();
    
    // Если отправитель не админ, добавить уведомление админу
    if (appData.currentUser.role !== 'admin') {
        addNotification(`Новое сообщение от ${appData.currentUser.username}`, 'message');
    }
}

// Инициация заказа (через кнопку "Заказать")
function initiateOrder() {
    if (!appData.currentUser) {
        alert('Для заказа услуг необходимо войти в аккаунт');
        showAccount();
        return;
    }
    
    openChat();
}

// Инициация заказа с конкретным тарифом
function initiateOrderWithPlan(planId) {
    if (!appData.currentUser) {
        alert('Для заказа услуг необходимо войти в аккаунт');
        showAccount();
        return;
    }
    
    // Автоматически выбрать тариф в чате
    selectedPlanForAttachment = planId;
    
    openChat();
    
    // Предзаполнить сообщение
    const plan = appData.plans.find(p => p.id == planId);
    if (plan) {
        document.getElementById('message-input').value = `Интересует тариф "${plan.name}". Подробнее о нем.`;
    }
}

// Добавление уведомления
function addNotification(text, type = 'info') {
    const notification = {
        id: appData.unreadNotifications.length + 1,
        text: text,
        type: type,
        timestamp: new Date(),
        read: false
    };
    
    appData.unreadNotifications.push(notification);
    
    // В реальном приложении здесь был бы код для показа уведомления пользователю
    console.log('Новое уведомление:', text);
}

// Загрузка уведомлений в админ панели
function loadAdminNotifications() {
    const notificationsContainer = document.getElementById('notification-list');
    notificationsContainer.innerHTML = '';
    
    if (appData.unreadNotifications.length === 0) {
        notificationsContainer.innerHTML = '<p>Нет новых уведомлений</p>';
        return;
    }
    
    appData.unreadNotifications.forEach(notification => {
        const notificationElement = document.createElement('div');
        notificationElement.className = `notification ${!notification.read ? 'unread' : ''}`;
        notificationElement.innerHTML = `
            <span>${notification.text}</span>
            <small>${formatDate(notification.timestamp)}</small>
        `;
        notificationsContainer.appendChild(notificationElement);
    });
}

// Форматирование даты
function formatDate(date) {
    return date.toLocaleString('ru-RU');
}

// Добавление нового тарифа через админ панель
function addNewPlan() {
    const name = document.getElementById('plan-name').value;
    const description = document.getElementById('plan-description').value;
    const price = document.getElementById('plan-price').value;
    
    if (!name || !description || !price) {
        alert('Пожалуйста, заполните все поля');
        return;
    }
    
    const newPlan = {
        id: appData.plans.length + 1,
        name: name,
        description: description,
        price: parseInt(price)
    };
    
    appData.plans.push(newPlan);
    
    // Очистка формы
    document.getElementById('plan-name').value = '';
    document.getElementById('plan-description').value = '';
    document.getElementById('plan-price').value = '';
    
    // Обновление списка тарифов на странице
    loadPricing();
    loadCurrentPricingList();
    
    alert('Тариф успешно добавлен');
}

// Загрузка текущего списка тарифов в админ панели
function loadCurrentPricingList() {
    const container = document.getElementById('current-pricing-list');
    container.innerHTML = '<h4>Текущие тарифы:</h4>';
    
    appData.plans.forEach(plan => {
        const planElement = document.createElement('div');
        planElement.className = 'price-card';
        planElement.innerHTML = `
            <h3>${plan.name}</h3>
            <div class="price">${plan.price} ₽</div>
            <p>${plan.description}</p>
            <button onclick="deletePlan(${plan.id})">Удалить</button>
        `;
        container.appendChild(planElement);
    });
}

// Удаление тарифа
function deletePlan(planId) {
    if (!confirm('Вы уверены, что хотите удалить этот тариф?')) {
        return;
    }
    
    appData.plans = appData.plans.filter(plan => plan.id !== planId);
    loadPricing();
    loadCurrentPricingList();
    updatePlanSelectOptions();
}