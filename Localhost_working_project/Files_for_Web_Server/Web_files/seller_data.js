var urlParams = new URLSearchParams(window.location.search);
var content = urlParams.get('content');
document.getElementById('display_data_para_id').textContent = content;