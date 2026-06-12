var xcmd=null,tmid1,tmid2,tmid3,isMoving=false;
var gauge = null;
function cmd(p)
{
    clearTimeout(tmid1);
    clearTimeout(tmid2);
    a=p||'';
    if(xcmd!=null)
    {
        xcmd.abort()
    }
    var pwr=gebi('pwr-slider').value;
    if(gauge)
    {
        gauge.setTargetValue(pwr);
        //gauge.setCurrentValue(pwr);
    }
    
    xcmd=new XMLHttpRequest();
    xcmd.onreadystatechange=()=>
    {
        if(xcmd.readyState==4&&xcmd.status==200)
        {
            clearTimeout(tmid1);
            clearTimeout(tmid2);
            
            var s=xcmd.responseText;
            var res=JSON.parse(s);
            var still=(res['pwr_current']===0)?true:false;
            gebi('pwr-value').innerHTML=s;
            if (still === isMoving)
            {
                isMoving = !still;
                disableCtrls(!still);
            }

            tmid2=setTimeout(cmd,2000);
        }
    };
    //xcmd.responseType = 'json';
    xcmd.open('GET','/tcontrol?pwr='+pwr+a,true);
    xcmd.send();
    tmid1=setTimeout(cmd,5000);
}

function changeDir()
{
    cmd('&dir='+gebi('dir-slider').value);
}

function toggleCab(el)
{
    var value = el.options[el.selectedIndex].value;
    var cab=(value==='cab1')?'0':'1';
    cmd('&cab='+cab);
}

function disableCtrls(val)
{
    gebi('dir-slider').disabled=val;
    gebi('cab').disabled=val;
}

function updGauge()
{
    var pwr=gebi('pwr-slider').value;
    if(gauge)
        gauge.setTargetValue(pwr);
}

function updCurrentPower()
{
    clearTimeout(tmid3);
    
    var dpwr = 0.125;
    var diff = gauge.tv - gauge.cv;
    var delta = 0;
    if (diff > 0)
    {
        delta=Math.min(dpwr, diff);
        gauge.setCurrentValue(gauge.cv+delta);
    }
    else if (diff < 0)
    {
        delta=Math.max(-3*dpwr, diff);
        gauge.setCurrentValue(gauge.cv+delta);
    }
    tmid3=setTimeout(updCurrentPower,25);
}

//

class GaugeClassic
{
    constructor(canvas)
    {
        this.opts={};
        this.cvs=canvas;

        this.cvs.style.width ='100%';
        this.cvs.style.height ='100%';
        this.minv=0;
        this.maxv=100;
        this.tv=this.cv=this.minv;
        this.smallStep=2;
        this.bigStep=10;
        this.stAng=-20;
        this.rangeAng=220;
        this.resize();
    }
    
    resize()
    {
        this.w = this.cvs.width = this.cvs.offsetWidth;
        this.h = this.cvs.height = this.cvs.offsetHeight;
    }
    
    draw()
    {
        var ctx=this.cvs.getContext('2d');
        ctx.save();
        var sz = Math.min(this.w,this.h);
        var ofs=15;
        ctx.clearRect(ofs, ofs, this.w-2*ofs, this.h-2*ofs);
        
        // h=57+textHeight/2 + 57*sin(this.toRad(-this.stAng))+textHeight/2
        // w=2*(57+textWidth/2)
        /*
        ctx.save();
        ctx.shadowBlur = ofs;
        ctx.shadowColor = '#555';
        ctx.fillStyle = '#555';
        ctx.fillRect(ofs, ofs, this.w-2*ofs, this.h-2*ofs);
        ctx.restore();
        */
        
        var sc = sz/120.0;
        ctx.translate(this.w/2.0, this.h*2.0/3.0);
        ctx.scale(sc, sc);

        ctx.strokeStyle = 'black';
        ctx.fillStyle = 'white';
        ctx.lineWidth = 1;
        ctx.lineCap = 'round';
        /*
        ctx.beginPath();
        ctx.arc(0, 0, 50, 0, Math.PI * 2, true);
        ctx.stroke();
        */
        // draw
        ctx.save();
        ctx.lineWidth = 0.8;
        this.drawScale(ctx, this.smallStep, 48, 45)
        ctx.restore();

        ctx.save();
        ctx.lineWidth = 1.3;
        this.drawScale(ctx, this.bigStep, 48, 42)
        ctx.restore();
        
        ctx.save();
        ctx.font = '5 serif';
        ctx.textAlign='center';
        ctx.textBaseline='middle';
        ctx.fillStyle = 'black';
        this.drawTextScale(ctx, this.bigStep, 57)
        ctx.restore();
        
        ctx.save();
        this.drawWheel(ctx, this.tv);
        ctx.restore();
        
        // draw hand shadow
        ctx.save();
        ctx.lineJoin = 'miter';
        ctx.lineWidth = 1;
        ctx.fillStyle = '#666';
        ctx.strokeStyle = '#666';
        ctx.filter = 'blur(3px)';
        ctx.translate(-1,-1);
        this.drawTextHand(ctx, 49, this.cv);
        ctx.restore();

        // draw hand
        ctx.save();
        ctx.lineJoin = 'miter';
        ctx.lineWidth = 1;
        ctx.fillStyle = 'red';
        ctx.strokeStyle = 'red';
        this.drawTextHand(ctx, 49, this.cv);
        ctx.restore();
        
        ctx.save();
        ctx.fillStyle='gold';
        ctx.beginPath();
        ctx.arc(0, 0, 1.3, 0, Math.PI * 2, true);
        ctx.fill();
        ctx.restore();

        ctx.restore();
    }
    
    drawScale(ctx, step, r1, r2)
    {
        ctx.rotate(this.toRad(this.stAng));
        var angStep=this.toRad(this.rangeAng*step/(this.maxv-this.minv));
        for (let v = this.minv; v <= this.maxv; v+=step) {
            ctx.beginPath();
            ctx.moveTo(-r1, 0);
            ctx.lineTo(-r2, 0);
            ctx.closePath();
            ctx.stroke();
            ctx.rotate(angStep);
        }
    }
    
    drawTextScale(ctx, step, r)
    {
        var angStep=this.toRad(this.rangeAng*step/(this.maxv-this.minv));
        var angRad = this.toRad(this.stAng);
        for (let v = this.minv; v <= this.maxv; v+=step, angRad+=angStep) {
            var x = -Math.cos(angRad)*r;
            var y = -Math.sin(angRad)*r;
            var s = Math.round(v).toString();
            ctx.fillText(s,x,y);
        }
    }

    drawTextHand(ctx, r, v)
    {
        var angRad=this.toRad(this.stAng+this.rangeAng*v/(this.maxv-this.minv));
        var r1=3;
        var r2=8;
        ctx.rotate(angRad);
        ctx.beginPath();
        var prevLW = ctx.lineWidth;
        ctx.lineWidth = prevLW*2;
        ctx.arc(0, 0, r1, 0, Math.PI * 2, true);
        ctx.stroke();
        ctx.lineWidth = prevLW;
        
        ctx.beginPath();
        ctx.moveTo(-r1, -r1/2.0);
        ctx.lineTo(-r, 0);
        ctx.lineTo(-r1, r1/2.0);
        ctx.closePath();
        //ctx.stroke();
        ctx.fill();
       
        ctx.beginPath();
        ctx.moveTo(r1, -r1/2.0);
        ctx.lineTo(r2, -r1/2.0);
        ctx.lineTo(r2, r1/2.0);
        ctx.lineTo(r1, r1/2.0);
        ctx.closePath();
        //ctx.stroke();
        ctx.fill();
    }
    
    drawWheel(ctx,v)
    {
        var stAng=Math.PI+this.toRad(this.stAng);
        var enAng=Math.PI+this.toRad(this.stAng+this.rangeAng);
        ctx.beginPath();
        ctx.strokeStyle = '#555';
        ctx.lineWidth = 6;
        ctx.shadowBlur = 10;
        ctx.shadowColor = '#555';
        ctx.arc(0, 0, 35, stAng, enAng, false);
        ctx.stroke();
        
        ctx.beginPath();
        ctx.strokeStyle = '#66e';
        ctx.lineWidth = 4;
        ctx.shadowBlur = 6;
        ctx.shadowColor = '#d6d';
        //stAng=Math.PI+this.toRad(this.stAng);
        enAng=Math.PI+this.toRad(this.stAng+this.rangeAng*v/(this.maxv-this.minv));
        ctx.beginPath();
        ctx.arc(0, 0, 35, stAng, enAng, false);
        ctx.stroke();

    }

    toRad(deg)
    {
        return Math.PI*deg/180.0;
    }
    
    setTargetValue(v)
    {
        if (v < this.minv)
            this.tv=this.minv;
        else if (v > this.maxv)
            this.tv=this.maxv;
        else
            this.tv=v;
    }
    
    setCurrentValue(v)
    {
        if (v < this.minv)
            this.cv=this.minv;
        else if (v > this.maxv)
            this.cv=this.maxv;
        else
            this.cv=v;
    }

}

function init()
{
    gauge = new GaugeClassic(gebi('gauge'));
    window.requestAnimationFrame( loop );
    updCurrentPower();
}

function loop()
{
    gauge.draw();
    window.requestAnimationFrame( loop );
}

function stop()
{
    gebi('confirmDlg').style.display = 'block';
}

window.addEventListener('resize', function(){
    gauge.resize();
});

wl(init);
wl(cmd);
