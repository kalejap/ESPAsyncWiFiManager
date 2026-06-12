/**
 * Remote OTA update module — served as /ota-remote.js when WM_REMOTE_UPDATE is defined.
 */
import { gebi } from './utils.js';
import { showRestartModal } from './restart.js';

function showInfoModal(msg) {
    var overlay = document.createElement('div');
    overlay.className = 'modal-overlay';
    overlay.innerHTML =
        '<div class=\'modal-box\'>' +
        '<p class=\'modal-msg\'>' + msg + '</p>' +
        '<div class=\'modal-btn-row\'>' +
        '<button class=\'modal-btn modal-btn-yes\' id=\'rmOk\'>OK</button>' +
        '</div></div>';
    document.body.appendChild(overlay);
    document.getElementById('rmOk').onclick = function() {
        document.body.removeChild(overlay);
    };
}

function showUpdateModal(data) {
    var langs = Object.keys(data.firmwares);
    var opts = langs.map(function(l) {
        var sel = (l === data.lang) ? ' selected' : '';
        return '<option value=\'' + l + '\'' + sel + '>' + l + '</option>';
    }).join('');

    var msg = data.available
        ? (/*L_OTA_REMOTE_NEW_VERSION*/'New version available: ' + '<strong>' + data.version + '</strong><br>' + /*L_OTA_REMOTE_CURRENT*/'Current: ' + data.current)
        : (/*L_OTA_REMOTE_SWITCH_LANG*/'Switch language (version ' + data.version + ')');

    var overlay = document.createElement('div');
    overlay.className = 'modal-overlay';
    overlay.innerHTML =
        '<div class=\'modal-box\'>' +
        '<p class=\'modal-msg\'>' + msg + '</p>' +
        '<div style=\'margin-bottom:0.75rem;\'>' +
        '<label class=\'flbl\' for=\'rmLang\'>' + /*L_OTA_REMOTE_LANGUAGE_LABEL*/'Language:' + '</label>' +
        '<select class=\'finp\' id=\'rmLang\'>' + opts + '</select>' +
        '</div>' +
        '<div class=\'modal-btn-row\'>' +
        '<button class=\'modal-btn modal-btn-yes\' id=\'rmUpdate\'>' + /*L_OTA_REMOTE_UPDATE_BTN*/'Update' + '</button>' +
        '<button class=\'modal-btn modal-btn-no\' id=\'rmBack\'>' + /*L_GENERAL_BACK*/'Back' + '</button>' +
        '</div></div>';
    document.body.appendChild(overlay);

    document.getElementById('rmUpdate').onclick = function() {
        var lang = document.getElementById('rmLang').value;
        var url  = data.firmwares[lang];
        document.body.removeChild(overlay);
        fetch('/ota/remote-start?url=' + encodeURIComponent(url))
            .then(function() {
                showRestartModal(30, /*L_OTA_UPDATE_IN_PROGRESS*/'Firmware update in progress.\nDo not turn off the device.');
            })
            .catch(function(e) {
                gebi('remoteStatus').textContent = /*L_OTA_REMOTE_START_FAILED*/'Start failed: ' + e.message;
            });
    };
    document.getElementById('rmBack').onclick = function() {
        document.body.removeChild(overlay);
    };
}

document.addEventListener('DOMContentLoaded', function() {
    gebi('checkRemoteBtn').addEventListener('click', async function() {
        var url = gebi('remoteUrl').value.trim();
        if (!url) return;
        gebi('remoteStatus').textContent = /*L_OTA_REMOTE_CHECKING*/'Checking...';
        try {
            var r = await fetch('/ota/remote-check?url=' + encodeURIComponent(url));
            var j = await r.json();
            if (j.error) {
                gebi('remoteStatus').textContent = /*L_OTA_REMOTE_ERROR*/'Error: ' + j.error;
                return;
            }
            gebi('remoteStatus').textContent = '';
            if (!j.available && !j.canSwitch) {
                showInfoModal(/*L_OTA_REMOTE_UP_TO_DATE*/'Firmware is up to date. Version: ' + j.current);
            } else {
                showUpdateModal(j);
            }
        } catch(e) {
            gebi('remoteStatus').textContent = /*L_OTA_REMOTE_CHECK_FAILED*/'Check failed: ' + e.message;
        }
    });
});
