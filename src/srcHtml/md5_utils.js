/**
 * MD5 implementation and related byte/word/encoding utilities.
 * The variables L and x, and the function A are part of the original bundling structure.
 */

function A(l){
    return l&&l.__esModule&&Object.prototype.hasOwnProperty.call(l,'default')?l.default:l
}
var L={exports:{}}, x={exports:{}};
(function(){
    var l='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/',
        f={
            rotl:function(s,d){return s<<d|s>>>32-d},
            rotr:function(s,d){return s<<32-d|s>>>d},
            endian:function(s){
                if(s.constructor==Number)
                    return f.rotl(s,8)&16711935|f.rotl(s,24)&4278255360;
                for(var d=0;d<s.length;d++)s[d]=f.endian(s[d]);
                return s
            },
            randomBytes:function(s){
                for(var d=[];s>0;s--)
                    d.push(Math.floor(Math.random()*256));
                return d
            },
            bytesToWords:function(s){
                for(var d=[],a=0,c=0;a<s.length;a++,c+=8)
                    d[c>>>5]|=s[a]<<24-c%32;
                return d
            },
            wordsToBytes:function(s){
                for(var d=[],a=0;a<s.length*32;a+=8)
                    d.push(s[a>>>5]>>>24-a%32&255);
                return d
            },
            bytesToHex:function(s){
                for(var d=[],a=0;a<s.length;a++)
                    d.push((s[a]>>>4).toString(16)),d.push((s[a]&15).toString(16));
                return d.join('')
            },
            hexToBytes:function(s){
                for(var d=[],a=0;a<s.length;a+=2)
                    d.push(parseInt(s.substr(a,2),16));
                return d
            },
            bytesToBase64:function(s){
                for(var d=[],a=0;a<s.length;a+=3)
                    for(var c=s[a]<<16|s[a+1]<<8|s[a+2],i=0;i<4;i++)
                        a*8+i*6<=s.length*8?d.push(l.charAt(c>>>6*(3-i)&63)):d.push('=');
                return d.join('')
            },
            base64ToBytes:function(s){
                s=s.replace(/[^A-Z0-9+\/]/ig,'');
                for(var d=[],a=0,c=0;a<s.length;c=++a%4)
                    c!=0&&d.push((l.indexOf(s.charAt(a-1))&Math.pow(2,-2*c+8)-1)<<c*2|l.indexOf(s.charAt(a))>>>6-c*2);
                return d
            }
        };
    x.exports=f
})();
var H=x.exports,
    F={
        utf8:{
            stringToBytes:function(l){
                return F.bin.stringToBytes(unescape(encodeURIComponent(l)))
            },
            bytesToString:function(l){
                return decodeURIComponent(escape(F.bin.bytesToString(l)))
            }
        },
        bin:{
            stringToBytes:function(l){
                for(var f=[],s=0;s<l.length;s++)
                    f.push(l.charCodeAt(s)&255);
                return f
            },
            bytesToString:function(l){
                for(var f=[],s=0;s<l.length;s++)
                    f.push(String.fromCharCode(l[s]));
                return f.join('')
            }
        }
    },
    I=F;
/*!
 * Determine if an object is a Buffer
 *
 * @author    Feross Aboukhadijeh <https://feross.org>
 * @license   MIT
 */
var O=function(l){
    return l!=null&&(M(l)||P(l)||!!l._isBuffer)
};
function M(l){
     return!!l.constructor&&typeof l.constructor.isBuffer=='function'&&l.constructor.isBuffer(l)
}
function P(l){
     return typeof l.readFloatLE=='function'&&typeof l.slice=='function'&&M(l.slice(0,0))
}
(function(){
    var l=H,f=I.utf8,s=O,d=I.bin,
        a=function(c,i){
            c.constructor==String?i&&i.encoding==='binary'?c=d.stringToBytes(c):c=f.stringToBytes(c):s(c)?c=Array.prototype.slice.call(c,0):!Array.isArray(c)&&c.constructor!==Uint8Array&&(c=c.toString());
            for(var r=l.bytesToWords(c),p=c.length*8,n=1732584193,e=-271733879,o=-1732584194,t=271733878,u=0;u<r.length;u++)
                r[u]=(r[u]<<8|r[u]>>>24)&16711935|(r[u]<<24|r[u]>>>8)&4278255360;r[p>>>5]|=128<<p%32,r[(p+64>>>9<<4)+14]=p;
            for(var m=a._ff,g=a._gg,h=a._hh,y=a._ii,u=0;u<r.length;u+=16){
                var C=n,S=e,k=o,_=t;
                n=m(n,e,o,t,r[u+0],7,-680876936),
                t=m(t,n,e,o,r[u+1],12,-389564586),
                o=m(o,t,n,e,r[u+2],17,606105819),
                e=m(e,o,t,n,r[u+3],22,-1044525330),
                n=m(n,e,o,t,r[u+4],7,-176418897),
                t=m(t,n,e,o,r[u+5],12,1200080426),
                o=m(o,t,n,e,r[u+6],17,-1473231341),
                e=m(e,o,t,n,r[u+7],22,-45705983),
                n=m(n,e,o,t,r[u+8],7,1770035416),
                t=m(t,n,e,o,r[u+9],12,-1958414417),
                o=m(o,t,n,e,r[u+10],17,-42063),
                e=m(e,o,t,n,r[u+11],22,-1990404162),
                n=m(n,e,o,t,r[u+12],7,1804603682),
                t=m(t,n,e,o,r[u+13],12,-40341101),
                o=m(o,t,n,e,r[u+14],17,-1502002290),
                e=m(e,o,t,n,r[u+15],22,1236535329),
                n=g(n,e,o,t,r[u+1],5,-165796510),
                t=g(t,n,e,o,r[u+6],9,-1069501632),
                o=g(o,t,n,e,r[u+11],14,643717713),
                e=g(e,o,t,n,r[u+0],20,-373897302),
                n=g(n,e,o,t,r[u+5],5,-701558691),
                t=g(t,n,e,o,r[u+10],9,38016083),
                o=g(o,t,n,e,r[u+15],14,-660478335),
                e=g(e,o,t,n,r[u+4],20,-405537848),
                n=g(n,e,o,t,r[u+9],5,568446438),
                t=g(t,n,e,o,r[u+14],9,-1019803690),
                o=g(o,t,n,e,r[u+3],14,-187363961),
                e=g(e,o,t,n,r[u+8],20,1163531501),
                n=g(n,e,o,t,r[u+13],5,-1444681467),
                t=g(t,n,e,o,r[u+2],9,-51403784),
                o=g(o,t,n,e,r[u+7],14,1735328473),
                e=g(e,o,t,n,r[u+12],20,-1926607734),
                n=h(n,e,o,t,r[u+5],4,-378558),
                t=h(t,n,e,o,r[u+8],11,-2022574463),
                o=h(o,t,n,e,r[u+11],16,1839030562),
                e=h(e,o,t,n,r[u+14],23,-35309556),
                n=h(n,e,o,t,r[u+1],4,-1530992060),
                t=h(t,n,e,o,r[u+4],11,1272893353),
                o=h(o,t,n,e,r[u+7],16,-155497632),
                e=h(e,o,t,n,r[u+10],23,-1094730640),
                n=h(n,e,o,t,r[u+13],4,681279174),
                t=h(t,n,e,o,r[u+0],11,-358537222),
                o=h(o,t,n,e,r[u+3],16,-722521979),
                e=h(e,o,t,n,r[u+6],23,76029189),
                n=h(n,e,o,t,r[u+9],4,-640364487),
                t=h(t,n,e,o,r[u+12],11,-421815835),
                o=h(o,t,n,e,r[u+15],16,530742520),
                e=h(e,o,t,n,r[u+2],23,-995338651),
                n=y(n,e,o,t,r[u+0],6,-198630844),
                t=y(t,n,e,o,r[u+7],10,1126891415),
                o=y(o,t,n,e,r[u+14],15,-1416354905),
                e=y(e,o,t,n,r[u+5],21,-57434055),
                n=y(n,e,o,t,r[u+12],6,1700485571),
                t=y(t,n,e,o,r[u+3],10,-1894986606),
                o=y(o,t,n,e,r[u+10],15,-1051523),
                e=y(e,o,t,n,r[u+1],21,-2054922799),
                n=y(n,e,o,t,r[u+8],6,1873313359),
                t=y(t,n,e,o,r[u+15],10,-30611744),
                o=y(o,t,n,e,r[u+6],15,-1560198380),
                e=y(e,o,t,n,r[u+13],21,1309151649),
                n=y(n,e,o,t,r[u+4],6,-145523070),
                t=y(t,n,e,o,r[u+11],10,-1120210379),
                o=y(o,t,n,e,r[u+2],15,718787259),
                e=y(e,o,t,n,r[u+9],21,-343485551),
                n=n+C>>>0,
                e=e+S>>>0,
                o=o+k>>>0,
                t=t+_>>>0
            }
            return l.endian([n,e,o,t])
        };
    a._ff=function(c,i,r,p,n,e,o){
        var t=c+(i&r|~i&p)+(n>>>0)+o;
        return(t<<e|t>>>32-e)+i
    },
    a._gg=function(c,i,r,p,n,e,o){
        var t=c+(i&p|r&~p)+(n>>>0)+o;
        return(t<<e|t>>>32-e)+i
    },
    a._hh=function(c,i,r,p,n,e,o){
        var t=c+(i^r^p)+(n>>>0)+o;
        return(t<<e|t>>>32-e)+i
    },
    a._ii=function(c,i,r,p,n,e,o){
        var t=c+(r^(i|~p))+(n>>>0)+o;
        return(t<<e|t>>>32-e)+i
    },
    a._blocksize=16,
    a._digestsize=16,
    L.exports=function(c,i){
        if(c==null)
            throw new Error(/*L_OTA_JS_ILEGAL_ARGUMENT*/'Illegal argument '+c);
        var r=l.wordsToBytes(a(c,i));
        return i&&i.asBytes?r:i&&i.asString?d.bytesToString(r):l.bytesToHex(r)
    }
})();
var U=L.exports;

// Export the MD5 function as R (main function in the original bundle context)
const R=A(U);

// Export MD5 utility
export { R };