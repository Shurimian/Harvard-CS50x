// Modifies the navbar as you scroll
window.addEventListener("scroll", function(){
    var header = document.getElementById("navBar");
    header.classList.toggle("sticky", window.scrollY > 0);
});

// Removes the active class if no longer hovering button (for contact pg)
// BROKEN: FIX IT
var activeButton = document.querySelectorAll(".btn-outline-primary:hover");
if (activeButton){
    for (let i = 0; i < activeButton.length; ++i){
        activeButton[i].addEventListener("mouseenter", function(){
            alert("Hello");
        });
    }
}
