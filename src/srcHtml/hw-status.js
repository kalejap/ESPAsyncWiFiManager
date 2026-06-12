(function(){
  var INTERVAL=10000;
  function refresh(){
    fetch('/sq?dx=hwstatus')
      .then(function(r){return r.json();})
      .then(function(data){
        var tbody=document.getElementById('hw-st-body');
        if(!tbody)return;
        if(!Array.isArray(data)||data.length===0){
          tbody.innerHTML='<tr><td colspan=\'3\' style=\'text-align:center;color:var(--muted)\'>&#8212;</td></tr>';
          return;
        }
        var h='';
        for(var i=0;i<data.length;i++){
          h+='<tr><td>'+(data[i].name||'')+'</td>'
            +'<td class=\'stval\'>'+data[i].value+'</td>'
            +'<td>'+(data[i].unit||'')+'</td></tr>';
        }
        tbody.innerHTML=h;
      })
      .catch(function(){});
  }
  document.addEventListener('DOMContentLoaded',function(){
    refresh();
    setInterval(refresh,INTERVAL);
  });
})();
