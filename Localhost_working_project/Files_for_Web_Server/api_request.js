function postData() {
    const apiUrl = 'http://localhost:8080/api/data';
    const dataToSend = '5';

    fetch(apiUrl, {
        method: 'POST',
        headers: {
            'Content-Type': 'text/plain'
        },
        body: dataToSend
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.text();
    })
    .then(data => {
        console.log('Response:', data);
    })
    .catch(error => {
        console.error('Error:', error);
    });
}