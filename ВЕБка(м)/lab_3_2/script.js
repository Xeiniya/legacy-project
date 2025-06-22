const flights = {};

function AddValue(key, value) {
    flights[key] = value;
    showMessage(`✅ Добавлено: <strong>${key}</strong><br>📌 Описание: ${value}`);
}

function DeleteValue(key) {
    if (flights[key]) {
        delete flights[key];
        showMessage(`❌ Рейс <strong>${key}</strong> удален.`);
    } else {
        showMessage(`⚠️ Рейс <strong>${key}</strong> не найден.`);
    }
}

function GetValueInfo(key) {
    return flights[key] ? `✈️ <strong>Рейс:</strong> ${key}<br>📌 <strong>Описание:</strong> ${flights[key]}` : "⚠️ Нет информации о рейсе.";
}

function ListValues() {
    if (Object.keys(flights).length === 0) {
        return "⚠️ Нет информации о рейсах.";
    }
    return Object.entries(flights)
        .map(([key, value]) => `✈️ <strong>Рейс:</strong> ${key}<br>📌 <strong>Описание:</strong> ${value}`)
        .join("<hr>");
}

function showMessage(message) {
    document.getElementById("output").innerHTML = message;
}

function handleAdd() {
    const key = prompt("Введите название рейса:");
    if (!key) return showMessage("⚠️ Отмена ввода.");
    
    const value = prompt("Введите описание рейса:");
    if (!value) return showMessage("⚠️ Описание не введено.");

    AddValue(key, value);
}

function handleDelete() {
    const key = prompt("Введите название рейса для удаления:");
    if (!key) return showMessage("⚠️ Отмена удаления.");
    
    DeleteValue(key);
}

function handleGetInfo() {
    const key = prompt("Введите название рейса:");
    if (!key) return showMessage("⚠️ Отмена запроса.");
    
    showMessage(GetValueInfo(key));
}

function handleListAll() {
    showMessage(ListValues());
}
