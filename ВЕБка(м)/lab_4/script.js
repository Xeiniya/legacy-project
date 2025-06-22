class THashStorage {
    constructor() {
        this.storage = {};
    }

    Reset() {
        this.storage = {};
        showMessage("📌 Хранилище очищено.");
    }

    AddValue(key, value) {
        this.storage[key] = value;
        showMessage(`✅ Добавлено: <strong>${key}</strong><br>📌 Описание: ${value}`);
    }

    GetValue(key) {
        return this.storage[key] || undefined;
    }

    DeleteValue(key) {
        if (key in this.storage) {
            delete this.storage[key];
            showMessage(`❌ Удалено: <strong>${key}</strong>`);
            return true;
        }
        showMessage(`⚠️ Рейс <strong>${key}</strong> не найден.`);
        return false;
    }

    GetKeys() {
        return Object.keys(this.storage);
    }
}

const flightStorage = new THashStorage();

function handleAdd() {
    const key = prompt("Введите название рейса:");
    if (!key) return showMessage("⚠️ Отмена ввода.");
    const value = prompt("Введите описание рейса:");
    if (!value) return showMessage("⚠️ Описание не введено.");
    flightStorage.AddValue(key, value);
}

function handleDelete() {
    const key = prompt("Введите название рейса для удаления:");
    if (!key) return showMessage("⚠️ Отмена удаления.");
    flightStorage.DeleteValue(key);
}

function handleGetInfo() {
    const key = prompt("Введите название рейса:");
    if (!key) return showMessage("⚠️ Отмена запроса.");
    const info = flightStorage.GetValue(key);
    showMessage(info ? `✈️ <strong>Рейс:</strong> ${key}<br>📌 <strong>Описание:</strong> ${info}` : "⚠️ Нет информации о рейсе.");
}

function handleListAll() {
    const keys = flightStorage.GetKeys();
    showMessage(keys.length ? `📋 Все рейсы:<br>${keys.join(", ")}` : "⚠️ Нет информации о рейсах.");
}

function showMessage(message) {
    document.getElementById("output").innerHTML = message;
}
