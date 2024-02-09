let check = document.getElementById('check_button_pincode') ;
check.addEventListener('click' , () =>{
    // console.log("Hello World");
    let pin_value = document.getElementById('pincode_check_id_1').value ;
    // console.log(pin_value) ;

    const apiUrl = 'http://localhost:8080/api/data';
    const dataToSend = pin_value;

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
        // console.log('Response:', data);
        if(data != null){
            let update_info = document.getElementById('deliverable_check_output_id') ;
            update_info.innerHTML = `<p class="deliverable_check_output_para">Sellers Available for this Pincode</p>
            <a href="http://www.google.co.in" id="seller_link" class="seller_details_button">View Sellers</a>` ;

            console.log('Response:', data);
        }
        else{
            let update_info = document.getElementById('deliverable_check_output_id') ;
            update_info.innerHTML = `<p class="deliverable_check_output_para_2">Unfortunately no sellers are available for this pincode.</p>` ;
            
            console.log("Null Output") ;
        }
    })
    .catch(error => {
        console.error('Error:', error);
    });


});

// function postData() {
//     const apiUrl = 'http://localhost:8080/api/data';
//     const dataToSend = '5';

//     fetch(apiUrl, {
//         method: 'POST',
//         headers: {
//             'Content-Type': 'text/plain'
//         },
//         body: dataToSend
//     })
//     .then(response => {
//         if (!response.ok) {
//             throw new Error('Network response was not ok');
//         }
//         return response.text();
//     })
//     .then(data => {
//         console.log('Response:', data);
//     })
//     .catch(error => {
//         console.error('Error:', error);
//     });
// }