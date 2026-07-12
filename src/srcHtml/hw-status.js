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
          if(data[i].type==='switch'){
            var on=data[i].state;
            var uid=data[i].uid;
            h+='<tr><td>'+(data[i].name||'')+'</td>'
              +'<td style=\'text-align:left;white-space:nowrap\'>'
              +'<button class=\'swbtn'+(on?'':' sw-active')+'\' data-uid=\''+uid+'\' data-st=\'0\' onclick=\'wm_sw(this.dataset.uid,+this.dataset.st)\'>O</button>'
              +'<button class=\'swbtn'+(on?' sw-active':'')+'\' data-uid=\''+uid+'\' data-st=\'1\' onclick=\'wm_sw(this.dataset.uid,+this.dataset.st)\'>I</button>'
              +'</td><td></td></tr>';
          } else {
            h+='<tr><td>'+(data[i].name||'')+'</td>'
              +'<td class=\'stval\'>'+data[i].value+'</td>'
              +'<td>'+(data[i].unit||'')+'</td></tr>';
          }
        }
        tbody.innerHTML=h;
      })
      .catch(function(){});
  }
  window.wm_sw=function(uid,state){
    fetch('/api/switch?id='+encodeURIComponent(uid)+'&state='+state)
      .then(function(){refresh();})
      .catch(function(){});
  };
  document.addEventListener('DOMContentLoaded',function(){
    refresh();
    setInterval(refresh,INTERVAL);
  });
})();
