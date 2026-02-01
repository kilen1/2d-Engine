// main.js - исправленная версия с кэшированием и сессиями

// Глобальные переменные
let currentChatId = null;
let selectedPlanForAttachment = null;
let portfolioData = [];
let pricingData = [];

// Инициализация при загрузке DOM
document.addEventListener('DOMContentLoaded', function() {
    console.log('DOM загружен, инициализация...');
    
    // Проверяем сессию при загрузке
    checkLoginStatus();
    
    // Загружаем данные
    loadPortfolio();
    loadPricing();
    
    // Настройка обработчиков событий
    setupEventListeners();
    
    // Настройка мобильного меню
    setupMobileMenu();
    
    // Плавная прокрутка для якорных ссылок
    setupSmoothScroll();
});

// Проверка статуса входа
async function checkLoginStatus() {
    try {
        const response = await fetch('api/auth/check_session.php', {
            method: 'GET',
            headers: {
                'Cache-Control': 'no-cache',
                'Pragma': 'no-cache'
            },
            cache: 'no-store'
        });
        
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        
        const data = await response.json();
        console.log('Статус сессии:', data);
        
        if (data.loggedIn) {
            updateUIForLoggedInUser(data.user);
        } else {
            // Убедимся, что пользователь отображается как неавторизованный
            updateUIForLoggedOutUser();
        }
    } catch (error) {
        console.error('Ошибка проверки сессии:', error);
        showToast('Ошибка соединения с сервером', 'error');
    }
}

// Обновление UI для авторизованного пользователя
function updateUIForLoggedInUser(user) {
    console.log('Обновление UI для пользователя:', user);
    
    const accountLink = document.getElementById('account-link');
    if (accountLink) {
        accountLink.innerHTML = `
            <a href="#account" onclick="showAccount(); return false;">
                <i class="fas fa-user"></i> ${user.username}
            </a>
        `;
    }
    
    // Обновляем информацию в модальном окне
    const userNameElement = document.getElementById('user-name');
    const userStatusElement = document.getElementById('user-status');
    
    if (userNameElement) {
        userNameElement.textContent = user.username;
    }
    
    if (userStatusElement) {
        userStatusElement.textContent = user.role === 'admin' ? 'администратор' : 'пользователь';
    }
    
    // Показываем панель управления
    const adminActions = document.getElementById('admin-actions');
    if (adminActions && user.role === 'admin') {
        adminActions.style.display = 'block';
    }
}

// Обновление UI для неавторизованного пользователя
function updateUIForLoggedOutUser() {
    const accountLink = document.getElementById('account-link');
    if (accountLink) {
        accountLink.innerHTML = `
            <a href="#account" onclick="showAccount(); return false;">Войти</a>
        `;
    }
}

// Загрузка портфолио
async function loadPortfolio() {
    try {
        const response = await fetch('api/portfolio/get_portfolio.php', {
            cache: 'no-store',
            headers: {
                'Cache-Control': 'no-cache'
            }
        });
        
        if (!response.ok) {
            throw new Error('Ошибка загрузки портфолио');
        }
        
        portfolioData = await response.json();
        renderPortfolio(portfolioData);
    } catch (error) {
        console.error('Ошибка загрузки портфолио:', error);
        const container = document.getElementById('portfolio-container');
        if (container) {
            container.innerHTML = `
                <div class="error-message">
                    <p>Не удалось загрузить портфолио. Пожалуйста, попробуйте позже.</p>
                </div>
            `;
        }
    }
}

// Отображение портфолио
function renderPortfolio(projects) {
    const container = document.getElementById('portfolio-container');
    if (!container) return;
    
    if (!projects || projects.length === 0) {
        container.innerHTML = `
            <div class="no-projects">
                <p>Проекты временно недоступны</p>
            </div>
        `;
        return;
    }
    
    container.innerHTML = projects.map(project => `
        <div class="portfolio-item" data-category="${project.category || 'website'}">
            <img src="${project.image_url || 'https://via.placeholder.com/400x300?text=Проект'}" 
                 alt="${project.title}" 
                 class="portfolio-img"
                 onerror="this.src='https://via.placeholder.com/400x300?text=Проект'">
            <div class="portfolio-info">
                <h3>${project.title}</h3>
                <p>${project.description}</p>
                ${project.category ? `<span class="category">${getCategoryName(project.category)}</span>` : ''}
            </div>
        </div>
    `).join('');
}

// Фильтрация портфолио
function filterPortfolio(category) {
    const buttons = document.querySelectorAll('.filter-btn');
    buttons.forEach(btn => btn.classList.remove('active'));
    event.target.classList.add('active');
    
    const items = document.querySelectorAll('.portfolio-item');
    items.forEach(item => {
        if (category === 'all' || item.dataset.category === category) {
            item.style.display = 'block';
        } else {
            item.style.display = 'none';
        }
    });
}

function getCategoryName(category) {
    const categories = {
        'website': 'Веб-сайт',
        'ecommerce': 'Интернет-магазин',
        'landing': 'Лендинг',
        'corporate': 'Корпоративный сайт'
    };
    return categories[category] || category;
}

// Загрузка тарифов
async function loadPricing() {
    try {
        const response = await fetch('api/plans/get_plans.php', {
            cache: 'no-store',
            headers: {
                'Cache-Control': 'no-cache'
            }
        });
        
        if (!response.ok) {
            throw new Error('Ошибка загрузки тарифов');
        }
        
        pricingData = await response.json();
        renderPricing(pricingData);
        updatePlanSelectOptions();
    } catch (error) {
        console.error('Ошибка загрузки тарифов:', error);
        const container = document.getElementById('pricing-container');
        if (container) {
            container.innerHTML = `
                <div class="error-message">
                    <p>Не удалось загрузить тарифы. Пожалуйста, попробуйте позже.</p>
                </div>
            `;
        }
    }
}

// Отображение тарифов
function renderPricing(plans) {
    const container = document.getElementById('pricing-container');
    if (!container) return;
    
    if (!plans || plans.length === 0) {
        container.innerHTML = `
            <div class="no-plans">
                <p>Тарифы временно недоступны</p>
            </div>
        `;
        return;
    }
    
    container.innerHTML = plans.map(plan => `
        <div class="price-card">
            <h3>${plan.name}</h3>
            <div class="price">${parseInt(plan.price).toLocaleString('ru-RU')} ₽</div>
            <ul class="price-features">
                ${plan.description.split(',').map(feature => `
                    <li>${feature.trim()}</li>
                `).join('')}
                <li>Поддержка 30 дней</li>
                <li>Адаптивный дизайн</li>
                <li>Гарантия качества</li>
            </ul>
            <button onclick="initiateOrderWithPlan(${plan.id})">Заказать</button>
        </div>
    `).join('');
}

// Обновление списка тарифов в чате
function updatePlanSelectOptions() {
    const select = document.getElementById('selected-plan');
    if (!select) return;
    
    select.innerHTML = '<option value="">Прикрепить тариф...</option>';
    
    if (pricingData && pricingData.length > 0) {
        pricingData.forEach(plan => {
            const option = document.createElement('option');
            option.value = plan.id;
            option.textContent = `${plan.name} - ${parseInt(plan.price).toLocaleString('ru-RU')} ₽`;
            select.appendChild(option);
        });
    }
}

// Настройка обработчиков событий
function setupEventListeners() {
    // Закрытие модальных окон
    const closeButtons = document.querySelectorAll('.close, .close-chat, .close-admin');
    closeButtons.forEach(btn => {
        btn.addEventListener('click', function() {
            this.closest('.modal').style.display = 'none';
        });
    });
    
    // Закрытие по клику вне модального окна
    window.addEventListener('click', function(event) {
        if (event.target.classList.contains('modal')) {
            event.target.style.display = 'none';
        }
    });
    
    // Обработка формы добавления тарифа
    const pricingForm = document.getElementById('pricing-form');
    if (pricingForm) {
        pricingForm.addEventListener('submit', function(e) {
            e.preventDefault();
            addNewPlan();
        });
    }
    
    // Обработка формы портфолио
    const portfolioForm = document.getElementById('portfolio-form');
    if (portfolioForm) {
        portfolioForm.addEventListener('submit', function(e) {
            e.preventDefault();
            addNewProject();
        });
    }
    
    // Обработка ввода сообщения в чате
    const messageInput = document.getElementById('message-input');
    if (messageInput) {
        messageInput.addEventListener('keypress', function(e) {
            if (e.key === 'Enter' && !e.shiftKey) {
                e.preventDefault();
                sendMessage();
            }
        });
    }
}

// Настройка мобильного меню
function setupMobileMenu() {
    const hamburger = document.querySelector('.hamburger');
    const navLinks = document.querySelector('.nav-links');
    
    if (hamburger && navLinks) {
        hamburger.addEventListener('click', function() {
            this.classList.toggle('active');
            navLinks.classList.toggle('active');
        });
        
        // Закрытие меню при клике на ссылку
        navLinks.querySelectorAll('a').forEach(link => {
            link.addEventListener('click', () => {
                hamburger.classList.remove('active');
                navLinks.classList.remove('active');
            });
        });
    }
}

// Плавная прокрутка
function setupSmoothScroll() {
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function(e) {
            const href = this.getAttribute('href');
            
            if (href === '#' || href === '#account') {
                return; // Пропускаем якорь аккаунта
            }
            
            e.preventDefault();
            
            const targetId = href.substring(1);
            const targetElement = document.getElementById(targetId);
            
            if (targetElement) {
                // Закрываем мобильное меню
                const hamburger = document.querySelector('.hamburger');
                const navLinks = document.querySelector('.nav-links');
                if (hamburger && navLinks) {
                    hamburger.classList.remove('active');
                    navLinks.classList.remove('active');
                }
                
                // Плавная прокрутка
                window.scrollTo({
                    top: targetElement.offsetTop - 80,
                    behavior: 'smooth'
                });
                
                // Обновляем активную ссылку
                document.querySelectorAll('.nav-links a').forEach(link => {
                    link.classList.remove('active');
                });
                this.classList.add('active');
            }
        });
    });
}

// Функции аккаунта
function showAccount() {
    const modal = document.getElementById('account-modal');
    if (modal) {
        modal.style.display = 'block';
        
        // Проверяем сессию при открытии
        checkLoginStatus().then(() => {
            // Показываем соответствующую форму
            const loginForm = document.getElementById('login-form');
            const registerForm = document.getElementById('register-form');
            const userDashboard = document.getElementById('user-dashboard');
            
            if (userDashboard && userDashboard.style.display === 'block') {
                // Уже авторизованы
                loginForm.style.display = 'none';
                registerForm.style.display = 'none';
                userDashboard.style.display = 'block';
            } else {
                // Не авторизованы
                loginForm.style.display = 'block';
                registerForm.style.display = 'none';
                userDashboard.style.display = 'none';
            }
        });
    }
}

function showRegister() {
    document.getElementById('login-form').style.display = 'none';
    document.getElementById('register-form').style.display = 'block';
}

function showLogin() {
    document.getElementById('register-form').style.display = 'none';
    document.getElementById('login-form').style.display = 'block';
}

// Вход в систему
async function login() {
    const username = document.getElementById('username').value.trim();
    const password = document.getElementById('password').value.trim();
    
    if (!username || !password) {
        showToast('Заполните все поля', 'error');
        return;
    }
    
    try {
        const response = await fetch('api/auth/login.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Cache-Control': 'no-cache'
            },
            body: JSON.stringify({ username, password }),
            cache: 'no-store'
        });
        
        const data = await response.json();
        
        if (data.success) {
            showToast('Вход выполнен успешно!', 'success');
            
            // Обновляем UI
            updateUIForLoggedInUser(data.user);
            
            // Переключаем на панель пользователя
            document.getElementById('login-form').style.display = 'none';
            document.getElementById('user-dashboard').style.display = 'block';
            
            // Закрываем модальное окно через 2 секунды
            setTimeout(() => {
                document.getElementById('account-modal').style.display = 'none';
            }, 2000);
            
        } else {
            showToast(data.message || 'Ошибка входа', 'error');
        }
    } catch (error) {
        console.error('Ошибка входа:', error);
        showToast('Ошибка соединения с сервером', 'error');
    }
}

// Регистрация
async function register() {
    const username = document.getElementById('reg-username').value.trim();
    const email = document.getElementById('reg-email').value.trim();
    const password = document.getElementById('reg-password').value.trim();
    
    if (!username || !email || !password) {
        showToast('Заполните все поля', 'error');
        return;
    }
    
    if (password.length < 6) {
        showToast('Пароль должен быть не менее 6 символов', 'error');
        return;
    }
    
    try {
        const response = await fetch('api/auth/register.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Cache-Control': 'no-cache'
            },
            body: JSON.stringify({ username, email, password }),
            cache: 'no-store'
        });
        
        const data = await response.json();
        
        if (data.success) {
            showToast('Регистрация успешна! Теперь войдите в аккаунт', 'success');
            showLogin();
            
            // Очищаем форму
            document.getElementById('reg-username').value = '';
            document.getElementById('reg-email').value = '';
            document.getElementById('reg-password').value = '';
        } else {
            showToast(data.message || 'Ошибка регистрации', 'error');
        }
    } catch (error) {
        console.error('Ошибка регистрации:', error);
        showToast('Ошибка соединения с сервером', 'error');
    }
}

// Выход из системы
async function logout() {
    try {
        const response = await fetch('api/auth/logout.php', {
            method: 'GET',
            headers: {
                'Cache-Control': 'no-cache'
            },
            cache: 'no-store'
        });
        
        const data = await response.json();
        
        if (data.success) {
            showToast('Выход выполнен', 'success');
            
            // Обновляем UI
            updateUIForLoggedOutUser();
            
            // Переключаем на форму входа
            document.getElementById('user-dashboard').style.display = 'none';
            document.getElementById('login-form').style.display = 'block';
            document.getElementById('register-form').style.display = 'none';
            
            // Закрываем модальное окно
            setTimeout(() => {
                document.getElementById('account-modal').style.display = 'none';
            }, 1500);
            
        } else {
            showToast('Ошибка выхода', 'error');
        }
    } catch (error) {
        console.error('Ошибка выхода:', error);
        showToast('Ошибка соединения с сервером', 'error');
    }
}

// Чат функции
async function openChat() {
    // Сначала проверяем авторизацию
    await checkLoginStatus();
    
    try {
        const response = await fetch('api/chat/start_chat.php', {
            method: 'GET',
            headers: {
                'Cache-Control': 'no-cache'
            },
            cache: 'no-store'
        });
        
        const data = await response.json();
        
        if (data.success) {
            currentChatId = data.chat_id;
            document.getElementById('chat-modal').style.display = 'block';
            loadChatMessages(data.chat_id);
        } else {
            showToast('Для использования чата необходимо войти в аккаунт', 'error');
            showAccount();
        }
    } catch (error) {
        console.error('Ошибка открытия чата:', error);
        showToast('Ошибка соединения с сервером', 'error');
    }
}

async function loadChatMessages(chatId) {
    if (!chatId) return;
    
    try {
        const response = await fetch(`api/chat/get_messages.php?chat_id=${chatId}&t=${Date.now()}`, {
            cache: 'no-store'
        });
        
        const messages = await response.json();
        const messagesContainer = document.getElementById('chat-messages');
        
        if (messagesContainer) {
            messagesContainer.innerHTML = '';
            
            if (messages.length === 0) {
                messagesContainer.innerHTML = `
                    <div class="welcome-message">
                        <p>Добро пожаловать в чат поддержки! Мы ответим вам в ближайшее время.</p>
                    </div>
                `;
            } else {
                messages.forEach(message => {
                    const messageElement = document.createElement('div');
                    messageElement.className = `message ${message.is_current_user ? 'user' : 'admin'}`;
                    
                    let messageHtml = `
                        <div class="message-content">${escapeHtml(message.message)}</div>
                        <div class="message-time">${new Date(message.created_at).toLocaleTimeString('ru-RU', { 
                            hour: '2-digit', 
                            minute: '2-digit' 
                        })}</div>
                    `;
                    
                    if (message.plan_name) {
                        messageHtml += `
                            <div class="attached-plan">
                                <i class="fas fa-tag"></i> ${message.plan_name} - ${parseInt(message.plan_price).toLocaleString('ru-RU')} ₽
                            </div>
                        `;
                    }
                    
                    messageElement.innerHTML = messageHtml;
                    messagesContainer.appendChild(messageElement);
                });
            }
            
            // Прокрутка вниз
            messagesContainer.scrollTop = messagesContainer.scrollHeight;
        }
    } catch (error) {
        console.error('Ошибка загрузки сообщений:', error);
    }
}

function attachPlanToMessage() {
    const select = document.getElementById('selected-plan');
    selectedPlanForAttachment = select.value;
}

async function sendMessage() {
    const messageInput = document.getElementById('message-input');
    const messageText = messageInput.value.trim();
    
    if (!messageText) {
        showToast('Введите сообщение', 'error');
        return;
    }
    
    try {
        const response = await fetch('api/chat/send_message.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Cache-Control': 'no-cache'
            },
            body: JSON.stringify({
                message: messageText,
                attached_plan_id: selectedPlanForAttachment
            }),
            cache: 'no-store'
        });
        
        const data = await response.json();
        
        if (data.success) {
            // Очищаем поле ввода
            messageInput.value = '';
            selectedPlanForAttachment = null;
            
            // Сбрасываем выбор тарифа
            const select = document.getElementById('selected-plan');
            if (select) select.value = '';
            
            // Обновляем сообщения
            await loadChatMessages(data.chat_id);
            
            // Фокус на поле ввода
            messageInput.focus();
        } else {
            showToast(data.message || 'Ошибка отправки сообщения', 'error');
        }
    } catch (error) {
        console.error('Ошибка отправки сообщения:', error);
        showToast('Ошибка соединения с сервером', 'error');
    }
}

// Функции заказа
function initiateOrder() {
    // Проверяем авторизацию
    checkLoginStatus().then(() => {
        try {
            const response = fetch('api/auth/check_session.php', {
                cache: 'no-store'
            }).then(res => res.json());
            
            response.then(data => {
                if (data.loggedIn) {
                    openChat();
                } else {
                    showToast('Для заказа необходимо войти в аккаунт', 'error');
                    showAccount();
                }
            });
        } catch (error) {
            console.error('Ошибка проверки сессии:', error);
            showToast('Ошибка соединения с сервером', 'error');
        }
    });
}

function initiateOrderWithPlan(planId) {
    // Проверяем авторизацию
    checkLoginStatus().then(() => {
        try {
            const response = fetch('api/auth/check_session.php', {
                cache: 'no-store'
            }).then(res => res.json());
            
            response.then(data => {
                if (data.loggedIn) {
                    // Находим план
                    const plan = pricingData.find(p => p.id == planId);
                    if (plan) {
                        // Выбираем план в чате
                        const select = document.getElementById('selected-plan');
                        if (select) {
                            select.value = planId;
                            attachPlanToMessage();
                        }
                        
                        // Открываем чат с предзаполненным сообщением
                        openChat();
                        
                        // Предзаполняем сообщение
                        const messageInput = document.getElementById('message-input');
                        if (messageInput) {
                            messageInput.value = `Здравствуйте! Меня интересует тариф "${plan.name}". Можете рассказать подробнее?`;
                            messageInput.focus();
                        }
                    }
                } else {
                    showToast('Для заказа необходимо войти в аккаунт', 'error');
                    showAccount();
                }
            });
        } catch (error) {
            console.error('Ошибка проверки сессии:', error);
            showToast('Ошибка соединения с сервером', 'error');
        }
    });
}

// Админ функции
async function openAdminPanel() {
    try {
        const response = await fetch('api/admin/check_admin.php', {
            cache: 'no-store'
        });
        
        const data = await response.json();
        
        if (data.is_admin) {
            document.getElementById('admin-panel').style.display = 'block';
            loadAdminStatistics();
            loadAdminNotifications();
            loadAllUsers();
            loadAllOrders();
            loadAllPlans();
            loadAllProjects();
        } else {
            showToast('Только администратор может открыть эту панель', 'error');
        }
    } catch (error) {
        console.error('Ошибка проверки прав администратора:', error);
        showToast('Ошибка соединения с сервером', 'error');
    }
}

async function loadAdminStatistics() {
    try {
        // Загружаем статистику
        // В реальном приложении здесь был бы отдельный endpoint
        const statsContainer = document.getElementById('admin-stats');
        if (statsContainer) {
            statsContainer.innerHTML = `
                <div class="stat-card">
                    <div class="stat-value">0</div>
                    <div class="stat-label">Пользователей</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value">0</div>
                    <div class="stat-label">Заказов</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value">0 ₽</div>
                    <div class="stat-label">Доход</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value">0</div>
                    <div class="stat-label">Активные чаты</div>
                </div>
            `;
        }
    } catch (error) {
        console.error('Ошибка загрузки статистики:', error);
    }
}

async function loadAdminNotifications() {
    try {
        const response = await fetch('api/admin/get_notifications.php', {
            cache: 'no-store'
        });
        
        const notifications = await response.json();
        const container = document.getElementById('notification-list');
        
        if (container) {
            if (notifications && notifications.length > 0) {
                container.innerHTML = notifications.map(notification => `
                    <div class="notification ${notification.is_read ? '' : 'unread'}">
                        <div>
                            <strong>${notification.message}</strong>
                            <div style="font-size: 12px; color: #666;">
                                ${notification.username ? `Пользователь: ${notification.username}` : ''}
                                ${new Date(notification.created_at).toLocaleString('ru-RU')}
                            </div>
                        </div>
                    </div>
                `).join('');
            } else {
                container.innerHTML = '<p>Нет уведомлений</p>';
            }
        }
    } catch (error) {
        console.error('Ошибка загрузки уведомлений:', error);
    }
}

async function loadAllUsers() {
    // Реализация загрузки пользователей
}

async function loadAllOrders() {
    try {
        const response = await fetch('api/admin/get_recent_orders.php', {
            cache: 'no-store'
        });
        
        const orders = await response.json();
        const container = document.getElementById('orders-list');
        
        if (container) {
            if (orders && orders.length > 0) {
                container.innerHTML = orders.map(order => `
                    <div class="order-item">
                        <div><strong>Заказ #${order.id}</strong></div>
                        <div>Пользователь: ${order.username}</div>
                        <div>Тариф: ${order.plan_name}</div>
                        <div>Сумма: ${parseInt(order.total_price).toLocaleString('ru-RU')} ₽</div>
                        <div>Статус: <span class="status-${order.status}">${getOrderStatusText(order.status)}</span></div>
                        <div>Дата: ${new Date(order.created_at).toLocaleString('ru-RU')}</div>
                    </div>
                `).join('');
            } else {
                container.innerHTML = '<p>Нет заказов</p>';
            }
        }
    } catch (error) {
        console.error('Ошибка загрузки заказов:', error);
    }
}

function getOrderStatusText(status) {
    const statuses = {
        'pending': 'Ожидает',
        'processing': 'В работе',
        'completed': 'Завершен',
        'cancelled': 'Отменен'
    };
    return statuses[status] || status;
}

async function loadAllPlans() {
    // Загружаем тарифы для админ панели
    try {
        const response = await fetch('api/plans/get_plans.php', {
            cache: 'no-store'
        });
        
        const plans = await response.json();
        const container = document.getElementById('current-pricing-list');
        
        if (container) {
            if (plans && plans.length > 0) {
                container.innerHTML = `
                    <h4>Текущие тарифы (${plans.length}):</h4>
                    <div class="plans-grid">
                        ${plans.map(plan => `
                            <div class="plan-card">
                                <h4>${plan.name}</h4>
                                <div class="price">${parseInt(plan.price).toLocaleString('ru-RU')} ₽</div>
                                <p>${plan.description}</p>
                                <div class="plan-actions">
                                    <button onclick="editPlan(${plan.id})" class="small-btn">Редактировать</button>
                                    <button onclick="deletePlan(${plan.id})" class="small-btn danger">Удалить</button>
                                </div>
                            </div>
                        `).join('')}
                    </div>
                `;
            } else {
                container.innerHTML = '<p>Нет тарифов</p>';
            }
        }
    } catch (error) {
        console.error('Ошибка загрузки тарифов:', error);
    }
}

async function loadAllProjects() {
    // Загрузка проектов для админ панели
}

async function addNewPlan() {
    const name = document.getElementById('plan-name').value.trim();
    const description = document.getElementById('plan-description').value.trim();
    const price = document.getElementById('plan-price').value;
    const features = document.getElementById('plan-features').value.trim();
    const isActive = document.getElementById('plan-active').checked;
    
    if (!name || !description || !price) {
        showToast('Заполните обязательные поля', 'error');
        return;
    }
    
    try {
        const response = await fetch('api/admin/manage_plans.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Cache-Control': 'no-cache'
            },
            body: JSON.stringify({
                name,
                description,
                price: parseFloat(price),
                features,
                is_active: isActive ? 1 : 0
            }),
            cache: 'no-store'
        });
        
        const data = await response.json();
        
        if (data.success) {
            showToast('Тариф успешно добавлен', 'success');
            
            // Очищаем форму
            document.getElementById('plan-name').value = '';
            document.getElementById('plan-description').value = '';
            document.getElementById('plan-price').value = '';
            document.getElementById('plan-features').value = '';
            
            // Обновляем список тарифов
            loadAllPlans();
            loadPricing(); // Обновляем на главной странице
        } else {
            showToast(data.message || 'Ошибка добавления тарифа', 'error');
        }
    } catch (error) {
        console.error('Ошибка добавления тарифа:', error);
        showToast('Ошибка соединения с сервером', 'error');
    }
}

async function addNewProject() {
    // Реализация добавления проекта
}

function switchAdminTab(tabName) {
    // Скрываем все вкладки
    document.querySelectorAll('.tab-content').forEach(tab => {
        tab.classList.remove('active');
    });
    
    // Убираем активный класс со всех кнопок
    document.querySelectorAll('.tab-btn').forEach(btn => {
        btn.classList.remove('active');
    });
    
    // Показываем выбранную вкладку
    const tabContent = document.getElementById(`admin-${tabName}`);
    if (tabContent) {
        tabContent.classList.add('active');
    }
    
    // Делаем кнопку активной
    const activeBtn = Array.from(document.querySelectorAll('.tab-btn')).find(btn => 
        btn.textContent.includes(getTabButtonText(tabName))
    );
    if (activeBtn) {
        activeBtn.classList.add('active');
    }
}

function getTabButtonText(tabName) {
    const texts = {
        'notifications': 'Уведомления',
        'users': 'Пользователи',
        'orders': 'Заказы',
        'pricing': 'Тарифы',
        'portfolio': 'Портфолио'
    };
    return texts[tabName] || tabName;
}

// Вспомогательные функции
function scrollToSection(sectionId) {
    const section = document.getElementById(sectionId);
    if (section) {
        window.scrollTo({
            top: section.offsetTop - 80,
            behavior: 'smooth'
        });
    }
}

function showToast(message, type = 'info') {
    const toastContainer = document.getElementById('notification-toast');
    if (!toastContainer) return;
    
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    toast.innerHTML = `
        <i class="fas fa-${type === 'success' ? 'check-circle' : 'exclamation-circle'}"></i>
        <span>${message}</span>
    `;
    
    toastContainer.appendChild(toast);
    
    // Удаляем тост через 5 секунд
    setTimeout(() => {
        toast.remove();
    }, 5000);
}

function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

// Показ настроек
function openSettings() {
    showToast('Настройки временно недоступны', 'info');
}

// Просмотр заказов пользователя
function viewMyOrders() {
    showToast('Просмотр заказов временно недоступен', 'info');
}

// Просмотр всех чатов (для администратора)
function viewAllChats() {
    showToast('Просмотр всех чатов временно недоступен', 'info');
}

// Установка заголовка с предотвращением кэширования
if (typeof Headers !== 'undefined') {
    // Добавляем заголовки для предотвращения кэширования
    const originalFetch = window.fetch;
    window.fetch = function(...args) {
        if (args[1]) {
            args[1].headers = {
                ...args[1].headers,
                'Cache-Control': 'no-cache, no-store, must-revalidate',
                'Pragma': 'no-cache',
                'Expires': '0'
            };
        }
        return originalFetch.apply(this, args);
    };
}