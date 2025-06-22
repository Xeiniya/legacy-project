document.addEventListener('DOMContentLoaded', function() {
    const contentContainer = document.getElementById('content');
    
    function loadPage(pageName) {
        contentContainer.innerHTML = '<div class="loading">Загрузка...</div>';
        const xhr = new XMLHttpRequest();
        const pagePath = pageName + '.html';
        xhr.open('GET', pagePath, true);
        
        xhr.onload = function() {
            if (xhr.status === 200) {
                contentContainer.innerHTML = xhr.responseText;
                initPageEventListeners();
                window.scrollTo(0, 0);
            } else {
                contentContainer.innerHTML = '<div class="container"><h2>Ошибка загрузки страницы</h2><p>Пожалуйста, попробуйте позже.</p></div>';
            }
        };
        
        xhr.onerror = function() {
            contentContainer.innerHTML = '<div class="container"><h2>Ошибка сети</h2><p>Не удалось загрузить страницу. Проверьте подключение к интернету.</p></div>';
        };
        xhr.send();
    }
    
    function initPageEventListeners() {
        const pageButtons = document.querySelectorAll('button[data-page]');
        
        pageButtons.forEach(button => {
            button.addEventListener('click', function() {
                const pageName = this.getAttribute('data-page');
                loadPage(pageName);
            });
        });
        
        const loadFlightsBtn = document.getElementById('loadFlightsBtn');
        if (loadFlightsBtn) {
            loadFlightsBtn.addEventListener('click', loadFlightsData);
        }
    }
    function loadFlightsData() {
        const loadingElement = document.getElementById('loadingFlights');
        const errorElement = document.getElementById('flightsError');
        const tableContainer = document.getElementById('flightsTableContainer');
        const loadFlightsBtn = document.getElementById('loadFlightsBtn');
        
        tableContainer.classList.add('hidden');
        errorElement.classList.add('hidden');
        loadingElement.classList.remove('hidden');
        loadFlightsBtn.disabled = true;
        
        const xhr = new XMLHttpRequest();
        
        xhr.open('GET', 'flights.json', true);
        
        xhr.responseType = 'json';
        
        xhr.onload = function() {
            loadingElement.classList.add('hidden');
            loadFlightsBtn.disabled = false;
            
            if (xhr.status === 200) {
                const data = xhr.response;
                
                if (data && data.flights && data.flights.length > 0) {
                    populateFlightsTable(data.flights);
                    tableContainer.classList.remove('hidden');
                } else {
                    errorElement.textContent = 'Нет доступных данных о рейсах.';
                    errorElement.classList.remove('hidden');
                }
            } else {
                errorElement.textContent = 'Ошибка при загрузке данных о рейсах. Пожалуйста, попробуйте позже.';
                errorElement.classList.remove('hidden');
            }
        };
        
        xhr.onerror = function() {
            loadingElement.classList.add('hidden');
            loadFlightsBtn.disabled = false;
            errorElement.textContent = 'Ошибка сети. Не удалось загрузить данные о рейсах.';
            errorElement.classList.remove('hidden');
        };
        
        xhr.send();
    }
    
    function populateFlightsTable(flights) {
        const tableBody = document.querySelector('#flightsTable tbody');
        
        tableBody.innerHTML = '';
        flights.forEach(flight => {
            const row = document.createElement('tr');
            const statusClass = flight.status.toLowerCase().includes('задерж') ? 'status-delayed' : '';
            
            row.innerHTML = `
                <td>${flight.id}</td>
                <td>${flight.departure}</td>
                <td>${flight.destination}</td>
                <td>${flight.departureTime}</td>
                <td>${flight.arrivalTime}</td>
                <td>${flight.aircraft}</td>
                <td class="${statusClass}">${flight.status}</td>
                <td>${flight.price.toLocaleString()} руб</td>
            `;
            tableBody.appendChild(row);
        });
    }
    
    initPageEventListeners();
    loadPage('home');
});