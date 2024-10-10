export default function useApi() {
    const baseUrl = 'http://192.168.1.1';  // Base URL per il server

    // Metodo GET generico
    // @ts-ignore
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
    // Metodo POST generico corretto
    const post = async (endpoint: string, data: Record<string, any>): Promise<{ status: number, message: string }> => {
        const url = `${baseUrl}${endpoint}`;

        try {
            const response = await fetch(url, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: new URLSearchParams(data).toString(),
            });

            const message = await response.text();  // Leggi il corpo della risposta come testo

            return {
                status: response.status,  // Restituisce lo stato HTTP
                message,  // Restituisce il corpo del messaggio
            };
        } catch (error) {
            console.error('Error in POST:', error);
            throw error;  // Solleva l'errore per la gestione esterna
        }
    };


    // Metodo specifico per setInternetTime corretto
    const setInternetTime = async (useInternetTime: boolean): Promise<{ status: number, message: string }> => {
        return post('/setInternetTime', { useInternetTime: useInternetTime.toString() });
    };

    const setDateTime = async (hour: number, minute: number, second: number, day: number, month: number, year: number): Promise<{ status: number, message: string }> => {
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
    const getTimeSetting = async (): Promise<{ status: number; message: string }> => {
        return get('/getTimeSetting');
    };
    const getSSID = async (): Promise<{ status: number; message: string }> => {
        return get('/SSID');
    }
    const getWifiPassword = async (): Promise<{ status: number; message: string }> => {
        return get('/Password');
    }

    const getSTA = async (): Promise<{ status: number; message: string }> => {
        return get('/useSTA');
    }

    const setWifiSettings = async (SSID: string, Password: string, useSTA: boolean): Promise<{ status: number, message: string }> => {
        return post('/setWifiSettings', { ssid: SSID, password: Password, useSTA: useSTA.toString() });
    }

    const getDimTime = async (): Promise<{ status: number, message: string }> => {
        return get('/getDimTimes');
    }

    const setDimTime = async (startDimTime: string, endDimTime): Promise<{ status: number, message: string }> => {
        return post('/setDimTimes', { startDimTime: startDimTime, endDimTime: endDimTime });
    }

    return {
        get,
        post,
        setInternetTime,
        setDateTime,
        getTimeSetting,
        getSSID,
        getWifiPassword,
        getSTA,
        setWifiSettings,
        getDimTime,
        setDimTime
    };
}
