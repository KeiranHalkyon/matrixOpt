let check = document.getElementById('check_button_pincode') ;
check.addEventListener('click' , () =>{
    let pin_value = document.getElementById('pincode_check_id_1').value ;
    
    let int_value = parseInt(pin_value) ;
    if(int_value >= 100000 && int_value <= 999999){
    
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
            let str_part = data.substring(0 , 2) ;
            let to_integer = parseInt(str_part) ;
            if(to_integer != -1){
            let update_info = document.getElementById('deliverable_check_output_id') ;
            update_info.innerHTML = `<p class="deliverable_check_output_para">Sellers Available for this Pincode</p>
            <a href="#" id="seller_link" class="seller_details_button">View Sellers</a>` ;
            console.log('Response:', data);
           // alert("The List of Sellers are: " + data);
            let view_details = document.getElementById('seller_link');
            view_details.addEventListener('click' , () =>{
               // window.location.href = "seller_data.html?content=" + encodeURIComponent(data);
               alert("The List of Sellers are: " + data);
            }) ;
        }
        else{
            alert("No Such Pincode Exist. Please enter a Valid Pincode.") ;
        }
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
    }
    else{
        alert("The Pincode you have entered is not Valid.") ;
    }

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