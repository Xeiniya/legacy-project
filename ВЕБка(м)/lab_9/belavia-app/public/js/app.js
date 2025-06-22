import { fromEvent, merge, of } from 'rxjs';
import { ajax } from 'rxjs/ajax';
import { 
  map, 
  switchMap, 
  catchError, 
  debounceTime, 
  distinctUntilChanged,
  tap,
  filter
} from 'rxjs/operators';

import '../../public/css/style.css';

document.addEventListener('DOMContentLoaded', function() {
  initUIAnimations();
  
  initFlightsTable();
  
  initFlightForm();
});

function initUIAnimations() {
  const flightsTable = document.querySelector('.flights-table');
  if (flightsTable) {
    flightsTable.style.opacity = '0';
    setTimeout(() => {
      flightsTable.style.transition = 'opacity 0.5s ease-in-out';
      flightsTable.style.opacity = '1';
    }, 100);
  }
}

function initFlightsTable() {
  const searchForm = document.querySelector('.search-form');
  const searchInput = searchForm?.querySelector('input[name="search"]');
  const sortLinks = document.querySelectorAll('.sort-link');
  const paginationLinks = document.querySelectorAll('.pagination-controls a, .per-page-selector a');
  
  if (searchInput) {
    const searchInputChange$ = fromEvent(searchInput, 'input').pipe(
      map(event => event.target.value),
      debounceTime(300), 
      distinctUntilChanged() 
    );
    
    searchInputChange$.subscribe(searchTerm => {
      if (searchTerm.length >= 3 || searchTerm.length === 0) {
        const currentUrl = new URL(window.location.href);
        currentUrl.searchParams.set('search', searchTerm);
        currentUrl.searchParams.set('page', '1'); 
        loadFlightsData(currentUrl.toString());
      }
    });
  }
  
  if (sortLinks.length > 0) {
    const sortClicks$ = Array.from(sortLinks).map(link => 
      fromEvent(link, 'click').pipe(
        tap(event => event.preventDefault()),
        map(event => event.currentTarget.href)
      )
    );
    
    merge(...sortClicks$).subscribe(url => {
      loadFlightsData(url);
    });
  }
  
  if (paginationLinks.length > 0) {
    const paginationClicks$ = Array.from(paginationLinks).map(link => 
      fromEvent(link, 'click').pipe(
        filter(event => !event.currentTarget.classList.contains('btn-disabled')),
        tap(event => event.preventDefault()),
        map(event => event.currentTarget.href)
      )
    );
    
    merge(...paginationClicks$).subscribe(url => {
      loadFlightsData(url);
    });
  }
  
  initDeleteButtons();
}

function loadFlightsData(url) {
  showLoading();
  
  const apiUrl = url.replace('/flights', '/api/flights');

  ajax.getJSON(apiUrl).pipe(
    catchError(error => {
      console.error('Error fetching flights:', error);
      hideLoading();
      return of({ error: 'Failed to load flights' });
    })
  ).subscribe(response => {
    if (response.error) {
      showError(response.error);
    } else {
      window.history.pushState({}, '', url);
      
      updateFlightsTable(response);
    }
    hideLoading();
  });
}

function updateFlightsTable(data) {
  const flightsContainer = document.querySelector('.flights-container');
  if (!flightsContainer) return;
  
  const urlParams = new URLSearchParams(window.location.search);
  const sortBy = urlParams.get('sortBy') || 'id';
  const sortOrder = urlParams.get('sortOrder') || 'asc';
  const search = urlParams.get('search') || '';
  
  const templateData = {
    flights: data.flights,
    pagination: {
      currentPage: data.currentPage,
      totalPages: data.totalPages,
      totalFlights: data.totalFlights,
      perPage: data.perPage,
      hasPrev: data.currentPage > 1,
      hasNext: data.currentPage < data.totalPages
    },
    sortBy: sortBy,
    sortOrder: sortOrder,
    search: search
  };
  
  let html = '';
  
  if (data.flights && data.flights.length > 0) {
    html += '<table class="flights-table"><thead><tr>';
    
    const headers = [
      { key: 'id', label: 'Номер рейса' },
      { key: 'departure', label: 'Откуда' },
      { key: 'destination', label: 'Куда' },
      { key: 'departureTime', label: 'Время вылета' },
      { key: null, label: 'Время прилета' },
      { key: null, label: 'Самолет' },
      { key: 'status', label: 'Статус' },
      { key: 'price', label: 'Цена' },
      { key: null, label: 'Действия' }
    ];
    
    headers.forEach(header => {
      html += '<th>';
      if (header.key) {
        const newSortOrder = (sortBy === header.key && sortOrder === 'asc') ? 'desc' : 'asc';
        html += `<a href="/flights?sortBy=${header.key}&sortOrder=${newSortOrder}&search=${search}&page=${templateData.pagination.currentPage}&limit=${templateData.pagination.perPage}" 
                  class="sort-link ${sortBy === header.key ? 'active' : ''}">
                  ${header.label}`;
        if (sortBy === header.key) {
          html += `<span class="sort-icon">${sortOrder === 'asc' ? '▲' : '▼'}</span>`;
        }
        html += '</a>';
      } else {
        html += header.label;
      }
      html += '</th>';
    });
    
    html += '</tr></thead><tbody>';
    
    data.flights.forEach(flight => {
      html += '<tr>';
      html += `<td>${flight.id}</td>`;
      html += `<td>${flight.departure}</td>`;
      html += `<td>${flight.destination}</td>`;
      html += `<td>${flight.departureTime}</td>`;
      html += `<td>${flight.arrivalTime}</td>`;
      html += `<td>${flight.aircraft}</td>`;
      html += `<td class="${flight.status.toLowerCase().includes('задерж') ? 'status-delayed' : ''}">${flight.status}</td>`;
      html += `<td>${new Intl.NumberFormat('ru-RU').format(flight.price)} руб</td>`;
      html += `<td class="actions">
                <a href="/flights/edit/${flight.id}" class="btn btn-small btn-edit">Изменить</a>
                <form action="/api/flights/${flight.id}/delete" method="POST" class="delete-form">
                  <button type="submit" class="btn btn-small btn-delete" data-id="${flight.id}">
                    Удалить
                  </button>
                </form>
              </td>`;
      html += '</tr>';
    });
    
    html += '</tbody></table>';
    
    if (templateData.pagination.totalPages > 1) {
      html += '<div class="pagination">';
      html += `<div class="pagination-info">
                Показано ${data.flights.length} из ${templateData.pagination.totalFlights} рейсов | 
                Страница ${templateData.pagination.currentPage} из ${templateData.pagination.totalPages}
              </div>`;
      
      html += '<div class="pagination-controls">';
      
      if (templateData.pagination.hasPrev) {
        html += `<a href="/flights?page=1&limit=${templateData.pagination.perPage}&sortBy=${sortBy}&sortOrder=${sortOrder}&search=${search}" 
                  class="btn btn-small btn-secondary">Первая</a>`;
        html += `<a href="/flights?page=${templateData.pagination.currentPage - 1}&limit=${templateData.pagination.perPage}&sortBy=${sortBy}&sortOrder=${sortOrder}&search=${search}" 
                  class="btn btn-small btn-secondary">Назад</a>`;
      } else {
        html += '<span class="btn btn-small btn-disabled">Первая</span>';
        html += '<span class="btn btn-small btn-disabled">Назад</span>';
      }
      
      let startPage = Math.max(1, templateData.pagination.currentPage - 2);
      let endPage = Math.min(templateData.pagination.totalPages, templateData.pagination.currentPage + 2);
      
      if (endPage - startPage < 4 && templateData.pagination.totalPages > 4) {
        if (startPage === 1) {
          endPage = Math.min(5, templateData.pagination.totalPages);
        } else if (endPage === templateData.pagination.totalPages) {
          startPage = Math.max(1, templateData.pagination.totalPages - 4);
        }
      }
      
      for (let i = startPage; i <= endPage; i++) {
        html += `<a href="/flights?page=${i}&limit=${templateData.pagination.perPage}&sortBy=${sortBy}&sortOrder=${sortOrder}&search=${search}" 
                 class="btn btn-small ${i === templateData.pagination.currentPage ? 'btn-primary' : 'btn-secondary'}">
                  ${i}
                </a>`;
      }
      
      if (templateData.pagination.hasNext) {
        html += `<a href="/flights?page=${templateData.pagination.currentPage + 1}&limit=${templateData.pagination.perPage}&sortBy=${sortBy}&sortOrder=${sortOrder}&search=${search}" 
                  class="btn btn-small btn-secondary">Вперед</a>`;
        html += `<a href="/flights?page=${templateData.pagination.totalPages}&limit=${templateData.pagination.perPage}&sortBy=${sortBy}&sortOrder=${sortOrder}&search=${search}" 
                  class="btn btn-small btn-secondary">Последняя</a>`;
      } else {
        html += '<span class="btn btn-small btn-disabled">Вперед</span>';
        html += '<span class="btn btn-small btn-disabled">Последняя</span>';
      }
      
      html += '</div>'; 
      
      html += '<div class="per-page-selector">';
      html += '<span>Показывать на странице:</span>';
      
      [5, 10, 20].forEach(limit => {
        html += `<a href="/flights?page=1&limit=${limit}&sortBy=${sortBy}&sortOrder=${sortOrder}&search=${search}" 
                 class="btn btn-small ${templateData.pagination.perPage === limit ? 'btn-primary' : 'btn-secondary'}">
                  ${limit}
                </a>`;
      });
      
      html += '</div>'; 
      html += '</div>'; 
    }
  } else {
    html += '<div class="empty-state">';
    if (search) {
      html += `<p>По запросу "${search}" рейсы не найдены. <a href="/flights">Сбросить поиск</a></p>`;
    } else {
      html += '<p>Рейсы не найдены. Добавьте новый рейс.</p>';
    }
    html += '</div>';
  }
  
  flightsContainer.innerHTML = html;
  
  initDeleteButtons();
}

function initDeleteButtons() {
  const deleteForms = document.querySelectorAll('.delete-form');
  
  if (deleteForms.length > 0) {
    const deleteClicks$ = Array.from(deleteForms).map(form => 
      fromEvent(form, 'submit').pipe(
        tap(event => event.preventDefault()),
        switchMap(event => {
          const confirmed = confirm('Вы уверены, что хотите удалить этот рейс?');
          const flightId = event.currentTarget.querySelector('button').dataset.id;
          
          if (confirmed) {
            showLoading();
            return ajax({
              url: `/api/flights/${flightId}`,
              method: 'DELETE'
            }).pipe(
              map(() => ({ success: true, originalEvent: event })),
              catchError(error => of({ success: false, error, originalEvent: event }))
            );
          } else {
            return of({ success: false, cancelled: true, originalEvent: event });
          }
        })
      )
    );
    
    merge(...deleteClicks$).subscribe(result => {
      hideLoading();
      
      if (result.success) {
        const currentUrl = window.location.href;
        loadFlightsData(currentUrl);
      } else if (!result.cancelled && result.error) {
        console.error('Error deleting flight:', result.error);
        showError('Не удалось удалить рейс');
      }
    });
  }
}

function initFlightForm() {
  const flightForm = document.querySelector('.flight-form');
  
  if (flightForm) {
    const formSubmit$ = fromEvent(flightForm, 'submit').pipe(
      tap(event => {
        const idInput = document.getElementById('id');
        const departureInput = document.getElementById('departure');
        const destinationInput = document.getElementById('destination');
        const priceInput = document.getElementById('price');
        
        let isValid = true;
        
        document.querySelectorAll('.form-group').forEach(group => {
          group.classList.remove('has-error');
          const errorMessage = group.querySelector('.form-error-message');
          if (errorMessage) errorMessage.remove();
        });
        
        if (idInput && !idInput.readOnly) {
          const idPattern = /^B2\d{3}$/;
          if (!idPattern.test(idInput.value)) {
            event.preventDefault();
            isValid = false;
            addErrorToField(idInput, 'Номер рейса должен соответствовать формату "B2XXX", где X - цифра.');
          }
        }
        
        if (departureInput.value === destinationInput.value) {
          event.preventDefault();
          isValid = false;
          addErrorToField(destinationInput, 'Пункт отправления и пункт назначения не могут быть одинаковыми.');
        }
        
        if (parseFloat(priceInput.value) < 1000) {
          event.preventDefault();
          isValid = false;
          addErrorToField(priceInput, 'Минимальная цена билета - 1000 руб.');
        }
        
        return isValid;
      })
    );
    
    formSubmit$.subscribe(isValid => {
      if (isValid) {
        
      }
    });
  }
}

function addErrorToField(inputElement, message) {
  const formGroup = inputElement.closest('.form-group');
  formGroup.classList.add('has-error');
  
  const errorMessage = document.createElement('div');
  errorMessage.className = 'form-error-message';
  errorMessage.textContent = message;
  
  formGroup.appendChild(errorMessage);
}

function showLoading() {
  hideLoading();
  
  const overlay = document.createElement('div');
  overlay.className = 'loading-overlay';
  overlay.innerHTML = '<div class="loading-spinner"></div>';
  document.body.appendChild(overlay);
}

function hideLoading() {
  const existingOverlay = document.querySelector('.loading-overlay');
  if (existingOverlay) {
    existingOverlay.remove();
  }
}

function showError(message) {
  let errorContainer = document.querySelector('.error-notification');
  
  if (!errorContainer) {
    errorContainer = document.createElement('div');
    errorContainer.className = 'error-notification';
    document.body.appendChild(errorContainer);
  }
  
  errorContainer.textContent = message;
  errorContainer.style.display = 'block';
  
  setTimeout(() => {
    errorContainer.style.display = 'none';
  }, 3000);
}