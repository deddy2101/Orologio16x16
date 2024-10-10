export default function useApi() {
    const baseUrl = 'http://tuo_indirizzo_ip_arduino';  // Base URL per il server

    // Metodo GET generico
    const get = async (endpoint: string): Promise<{ status: number, message: string }> => {
        const url = `${baseUrl}${endpoint}`;

        try {
            const response = await fetch(url, {
                method: 'GET',
                headers: {
                    'Content-Type': 'text/plain',  // Aspettiamo una risposta di tipo text/plain
                },
            });

            const message = await response.text();  // Raccoglie il corpo della risposta come testo
            return { status: response.status, message };
        } catch (error) {
            console.error('Error in GET:', error);
            throw error;
        }
    };

    // Metodo POST generico
    const post = async (endpoint: string, data: Record<string, any>): Promise<any> => {
        const url = `${baseUrl}${endpoint}`;

        try {
            const response = await fetch(url, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: new URLSearchParams(data).toString(),
            });

            if (response.ok) {
                return await response.text();  // Restituisce il messaggio di successo
            } else {
                throw new Error(`POST request failed: ${response.statusText}`);
            }
        } catch (error) {
            console.error('Error in POST:', error);
            throw error;
        }
    };

    // Metodo specifico per setInternetTime
    const setInternetTime = async (useInternetTime: boolean): Promise<string> => {
        return post('/setInternetTime', { useInternetTime: useInternetTime.toString() });
    };

    // Metodo specifico per setDateTime
    const setDateTime = async (hour: number, minute: number, second: number, day: number, month: number, year: number): Promise<string> => {
        const data = {
            hour: hour.toString(),
            minute: minute.toString(),
            second: second.toString(),
            day: day.toString(),
            month: month.toString(),
            year: year.toString(),
        };

        return post('/setDateTime', data);
    };

    // Metodo specifico per ottenere le impostazioni del tempo
    const getTimeSetting = async (): Promise<string> => {
        return get('/getTimeSetting');
    };

    return {
        get,
        post,
        setInternetTime,
        setDateTime,
        getTimeSetting,
    };
}
