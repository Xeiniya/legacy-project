const fs = require('fs').promises;
const path = require('path');

const dbPath = path.join(__dirname, 'db.json');

async function readData() {
    try {
        const data = await fs.readFile(dbPath, 'utf8');
        return JSON.parse(data);
    } catch (error) {
        console.error('Ошибка при чтении файла:', error);
        return { flights: [] };
    }
}

async function writeData(data) {
    try {
        await fs.writeFile(dbPath, JSON.stringify(data, null, 2), 'utf8');
        return true;
    } catch (error) {
        console.error('Ошибка при записи в файл:', error);
        return false;
    }
}

async function getAllFlights() {
    const data = await readData();
    return data.flights;
}

async function getFlightById(id) {
    const data = await readData();
    return data.flights.find(flight => flight.id === id);
}

async function addFlight(flight) {
    const data = await readData();
    
    if (data.flights.some(f => f.id === flight.id)) {
        throw new Error('Рейс с таким ID уже существует');
    }
    
    data.flights.push(flight);
    await writeData(data);
    return flight;
}

async function updateFlight(id, updatedFlight) {
    const data = await readData();
    const index = data.flights.findIndex(flight => flight.id === id);
    
    if (index === -1) {
        throw new Error('Рейс не найден');
    }
    
    updatedFlight.id = id;
    data.flights[index] = updatedFlight;
    
    await writeData(data);
    return updatedFlight;
}

async function deleteFlight(id) {
    const data = await readData();
    const index = data.flights.findIndex(flight => flight.id === id);
    
    if (index === -1) {
        throw new Error('Рейс не найден');
    }
    
    const deletedFlight = data.flights[index];
    data.flights.splice(index, 1);
    
    await writeData(data);
    return deletedFlight;
}

module.exports = {
    getAllFlights,
    getFlightById,
    addFlight,
    updateFlight,
    deleteFlight
};