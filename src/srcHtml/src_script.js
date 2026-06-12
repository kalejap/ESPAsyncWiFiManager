qs=s=>document.querySelector(s);
gebi=s=>document.getElementById(s);
wl=f=>window.addEventListener('load',f);
function c(l){
    gebi('s').value=l.innerText||l.textContent;
    gebi('p').focus();
    gebi('s1').value=l.innerText||l.textContent;
    gebi('p1').focus();
    gebi('timezone').value=timezone.name();
}
