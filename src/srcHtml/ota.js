/**
 * Main application logic for OTA update.
 */

// Import necessary utilities
import { gebi, IH, } from './utils.js';
// Import the MD5 hash function
import { R } from './md5_utils.js';
import { showRestartModal } from './restart.js';

// UI helper: make element visible (remove 'hidden' class)
const v=l=>{
    gebi(l).classList.remove('hidden');
};

// UI helper: hide element (add 'hidden' class)
const H=l=>{
    gebi(l).classList.add('hidden');
};

// UI helper: set progress title text
const w=l=>{
    gebi('progInfo').textContent = l;
};

// UI helper: set error title text
const E=l=>{
    gebi('err').textContent = l;
};

/**
 * Searches a byte array for a specific tag and extracts the null-terminated string identifier
 * immediately following the tag.
 * @param {ArrayBuffer} buf - The ArrayBuffer containing the file data.
 * @param {string} tag - The string tag to search for.
 * @returns {string | null} The extracted identifier string or null if not found.
 */
function fid(buf, tag) {
    const b = new Uint8Array(buf);
    const tb = new TextEncoder().encode(tag);
    for (let i = 0; i <= b.length - tb.length; i++) {
        let found = true;
        for (let j = 0; j < tb.length; j++) {
            if (b[i + j] !== tb[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            // Identifier starts after tag
            let ib = [];
            let k = i + tb.length;
            while (k < b.length && b[k] !== 0) {
                ib.push(b[k]);
                k++;
            }
            return new TextDecoder().decode(new Uint8Array(ib));
        }
    }
    return null;
}

/**
 * Reads a file and calculates MD5, extracts Hardware ID (hwid) and Firmware Version (ver).
 * @param {File} f - The file object to read.
 * @returns {Promise<{md5: string, hwid: string | null, ver: string | null}>} An object containing the MD5, hwid, and ver.
 */
const D1 = async (f) => new Promise((resolve, reject) => {
    let reader = new FileReader();
    reader.onload = function(event) {
        const arrayBuffer = event.target.result;
        const md5 = R(arrayBuffer); // R is the imported MD5 function
        const hwid = fid(arrayBuffer, '@*MAGic*@:hw:');
        const ver  = fid(arrayBuffer, '@*MAGic*@:ve:');
        const lang = fid(arrayBuffer, '@*MAGic*@:lg:');
        resolve({ md5, hwid, ver, lang });
    };
    reader.onerror = reject;
    reader.readAsArrayBuffer(f);
});

/**
 * Main function to process the selected firmware file and prepare for upload.
 * @param {File} l - The firmware file object.
 */
const N = async l => {
    //B('uploadColumn'),B('settingsColumn'),v('progressColumn'); // Commented out in original, keeping it this way.
    let f='fr'; /*gebi('otaMode').value;*/ // Mode fixed to 'fr'
    const { md5, hwid, ver, lang }=await D1(l);
    if(hwid!=null && ver!=null)
    {
      v('fileProps');
      IH('fwName',l.name);
      IH('filesize',l.size.toString() + ' B');
      IH('hwid',hwid);
      IH('fmver',ver);
      IH('fmlang',lang!=null?lang:'');
      H('successRow');
      try {
        const dR = await fetch('/sq?dx=hwid');
        const dJ = await dR.json();
        if (dJ.hwid && dJ.hwid === hwid) {
          gebi('hwid').classList.remove('red-txt');
          gebi('updateBtn').disabled = false;
          gebi('err').textContent = '';
          H('errRow');
        } else {
          gebi('hwid').classList.add('red-txt');
          gebi('updateBtn').disabled = true;
          E(/*L_OTA_JS_HWID_MISMATCH*/'Hardware ID mismatch. This firmware is not for this device.');
          v('errRow');
          return null;
        }
      } catch(e) {
        gebi('updateBtn').disabled = true;
        E(/*L_OTA_JS_HWID_MISMATCH*/'Hardware ID mismatch. This firmware is not for this device.');
        v('errRow');
        return null;
      }
      return {'md5': md5, 'file': l, 'hwid': hwid };
    }
    else
    {
      H('fileProps');
      IH('fwName',l.name);
      gebi('updateBtn').disabled = true;
      E('Incorrect file selected.');
      H('successRow');
      v('errRow');
    }
    
    return null; // Invalid file selected.
};

/**
 * Performs basic file validation (single file, .bin extension).
 * @param {FileList} l - The FileList object.
 * @returns {boolean} True if validation passes, false otherwise.
 */
const V = l => l.length>1&&!multiple?(alert(/*L_OTA_JS_UPLOAD_ONE_BIN_FILE*/'You can only upload one (.bin) file at a time.'),!1):l[0].name.split('.').pop()!='bin'?(alert(/*L_OTA_JS_UPLOAD_ONLY_BIN_FILES*/'You can only upload (.bin) files.'),!1):!0;

/**
 * Reloads the current page (used for reset).
 */
function G(){
    window.location.reload()
}

function PG(v){
    gebi('otaProg').value=v;
    gebi('otaProgTxt').innerHTML=v.toString()+'%';
}

var timeOut;

function updateProgress() {
  var prog = gebi('otaProg');
  if (prog.value >= 100) {
    //stop running this function after value reaches 100 (percent)
    PG(100);
    clearTimeout(timeOut);
    //finishedOK();
    failed('Updated failed');
    return;
  }
  PG(prog.value+10);
  timeOut = setTimeout(updateProgress, 500);
}

function finishedOK(){
  H('progRow');
  showRestartModal(15, /*L_OTA_UPDATE_IN_PROGRESS*/'Firmware update in progress.\nDo not turn off the device.');
}

function failed(msg){
  E(msg);
  gebi('updateBtn').disabled = false;
  gebi('back').disabled = false;
  H('progRow');
  v('errRow');
}

/**
 * Initiates the OTA update process (called on start button click).
 * This function should be modified to use the file/md5 data from N.
 * The logic below is adapted from the latter part of the original function N.
 * @param {File} file - The file to upload.
 * @param {string} md5 - The MD5 hash of the file.
 * @param {string} hwid - Hardware ID from the file.
 */
async function startUpload(file, md5, hwid) {
    let f='fr'; // otaMode, fixed as per original N function

    PG(0);
    H('errRow');
    H('successRow');
    v('progRow');
        
    try {
        w('Uploading '+file.name);

        //clearInterval(timeOut);
        //timeOut = setTimeout(updateProgress, 500);
        //return;
        
        const d=await fetch(`/ota/start?mode=${f}&hash=${md5}&hwid=${hwid}`);
        if(!d.ok)
        {
          throw new Error(d.statusText);
        }
        
        if (d.status == 200 && d.statusText !== 'OK')
        {
          throw new Error(d.statusText);
        }
            
        const a=await d.text();
        console.log('Start OTA response:',a);
        
        const c=new FormData;
        let i=new XMLHttpRequest;
        
        i.open('POST','/ota/upload');
        
        i.upload.addEventListener('progress',function(r){
            let p=Math.round(r.loaded/r.total*100);
            PG(p);
        },!1);
        
        i.upload.onprogress=function(r){
            if(r.lengthComputable){
                let p=Math.round(r.loaded/r.total*100);
                PG(p);
            }
        };
        
        i.onreadystatechange=function(){
            if(i.readyState==4)
                if(i.status==200)
                  finishedOK();
                else if(i.status==400){
                  let r=i.responseText;
                  throw new Error(/*L_OTA_JS_UPLOAD_FAILED*/'Upload failed.'+' '+r);
                }
                else{
                  let r=/*L_OTA_JS_SERVER_RETURNED_STATUS*/'Server returned status code '+i.status;
                  throw new Error(/*L_OTA_JS_UPLOAD_FAILED*/'Upload failed.'+' '+r);
                }
        };
        
        c.append('file',file,file.name);
        i.send(c);
    }
    catch(s){
        //B('progressColumn'),
        //v('errorColumn'),
        failed(s);
    }
}


// The file state needs to be maintained to be used by the start button.
// This is a global/module-scoped variable to hold the selected file and its MD5.
let selectedFile = null;

async function z(files){
  if(!V(files))
    return false;

  const result = await N(files[0]);
  if (result!=null) {
    selectedFile = result;
  } else {
    selectedFile = null;
  }
  gebi('fileInput').value = '';
}

// Event listeners and window exports
window.resetView=G;
window.onFileInput=z;

// Original event listener logic adapted for module structure
document.addEventListener('DOMContentLoaded', () => {
    var q=gebi('selFile'),fileInp=gebi('fileInput'); // q=selFile, $=fileInput
    const updateBtn = gebi('updateBtn'); // updateBtn is used in the main function N

    q.addEventListener('click',function(l){
        l.preventDefault(),fileInp.click()
    });

    // The file input's onchange calls z (onFileInput), which in turn calls N.
    // N will now save the file/md5 data and enable the start button.
    // We need to re-wire z/onFileInput to capture the results of N.
    
    // Override the function z (onFileInput) to store the result from N.
    /*
    window.onFileInput = async function(files) {
        if(!V(files)) return false;
        
        const result = await N(files[0]);
        if (result) {
            selectedFile = result;
        } else {
            selectedFile = null;
        }
    };
    */
    
    // Handle the start button click
    updateBtn.addEventListener('click', async (e) => {
        e.preventDefault();
        if (selectedFile && selectedFile.file && selectedFile.md5) {
            updateBtn.disabled = true; // Disable to prevent double-click
            gebi('back').disabled = true; // Disable back button
            startUpload(selectedFile.file, selectedFile.md5, selectedFile.hwid);
        } else {
            console.error('No valid file selected for upload.');
            alert('Please select a valid firmware file first.');
        }
    });
});