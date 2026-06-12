/**
 * Module Preload Polyfill/Loader (Bundler-generated code).
 */

(
    function(){
        const f=document.createElement('link').relList;
        if (f&&f.supports&&f.supports('modulepreload'))
            return;
        for(const a of document.querySelectorAll('link[rel=\'modulepreload\']'))d(a);
        new MutationObserver(
            a=>{
                for(const c of a)
                    if(c.type==='childList')
                        for(const i of c.addedNodes)
                            i.tagName==='LINK'&&i.rel==='modulepreload'&&d(i)
            }
        ).observe(document,{childList:!0,subtree:!0});
        function s(a){
            const c={};
            return a.integrity && (c.integrity=a.integrity),a.referrerPolicy && (c.referrerPolicy=a.referrerPolicy),a.crossOrigin==='use-credentials'?c.credentials='include':a.crossOrigin==='anonymous'?c.credentials='omit':c.credentials='same-origin',c
        }
            
        function d(a){
            if(a.ep)
                return;
            a.ep=!0;
            const c=s(a);
            fetch(a.href,c)
        }
    }
)();