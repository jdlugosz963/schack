document.getElementById("button-previous-s").onclick = function() {
  fetch("/previouss");
}

document.getElementById("button-previous").onclick = function() {
  fetch("/previous");
}

document.getElementById("button-next").onclick = function() {
  fetch("/next");
}

document.getElementById("button-next-s").onclick = function() {
  fetch("/nexts");
}
