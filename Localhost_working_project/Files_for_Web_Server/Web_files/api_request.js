let check = document.getElementById('check_button_pincode') ;
check.addEventListener('click' , () =>{
    let pin_value = document.getElementById('pincode_check_id_1').value ;
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
        if(data != null){
            let update_info = document.getElementById('deliverable_check_output_id') ;
            update_info.innerHTML = `<p class="deliverable_check_output_para">Sellers Available for this Pincode</p>
            <a href="./seller_list.html" id="seller_link" class="seller_details_button">View Sellers</a>` ;
            console.log('Response:', data);
           // alert("The List of Sellers are: " + data);
            let view_details = document.getElementById('seller_link');
            view_details.addEventListener('click' , () =>{
               // window.location.href = "seller_data.html?content=" + encodeURIComponent(data);
               alert("The List of Sellers are: " + data);
            }) ;
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