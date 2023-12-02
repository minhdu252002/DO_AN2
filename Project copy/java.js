//Thiết lập thời gian--------------------------------------------//
function updateTime() {
    var currentDate = new Date();
    var hours = currentDate.getHours();
    var minutes = currentDate.getMinutes();
    var seconds = currentDate.getSeconds();
    var day = currentDate.getDate();
    var month = currentDate.getMonth() + 1;
    var year = currentDate.getFullYear();
    var timeString = formatTime(hours) + ":" + formatTime(minutes) + ":" + formatTime(seconds);
    var dateString = formatTime(day) + "/" + formatTime(month) + "/" + year;
    var timeElements = document.getElementsByClassName("time");
    var dateElements = document.getElementsByClassName("date");
    for (var i = 0; i < timeElements.length; i++) {
      timeElements[i].innerHTML = timeString;
    }
    for (var j = 0; j < dateElements.length; j++) {
      dateElements[j].innerHTML = dateString;
    }
  }
function formatTime(time) {
    return time < 10 ? "0" + time : time;
}
