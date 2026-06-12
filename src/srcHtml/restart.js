var _modalCss =
    '.modal-overlay{position:fixed;inset:0;background:rgba(0,0,0,0.55);display:flex;align-items:center;justify-content:center;z-index:9999}' +
    '.modal-box{background:#fff;border-radius:.75rem;box-shadow:0 8px 32px rgba(0,0,0,.28);padding:2.5rem 2rem;text-align:center;min-width:220px}' +
    '.spinner{width:52px;height:52px;border:5px solid rgba(25,118,210,.15);border-top-color:#1976d2;border-radius:50%;animation:spin .8s linear infinite;margin:0 auto 1.25rem}' +
    '@keyframes spin{to{transform:rotate(360deg)}}' +
    '.modal-msg{color:#718792;margin-bottom:.75rem;font-size:1.05rem;white-space:pre-line}' +
    '.modal-cnt{font-size:2.5rem;font-weight:700;color:#1976d2}' +
    '.modal-btn-row{display:flex;gap:.75rem;justify-content:center;margin-top:1.25rem}' +
    '.modal-btn{padding:.6rem 1.5rem;border:none;border-radius:.5rem;font-size:1rem;font-weight:600;cursor:pointer}' +
    '.modal-btn-yes{background:#1976d2;color:#fff}' +
    '.modal-btn-no{background:transparent;color:#718792;border:.125rem solid rgba(25,118,210,.2)}';

function ensureModalCss() {
    if (!document.getElementById('rModalCss')) {
        var s = document.createElement('style');
        s.id = 'rModalCss';
        s.textContent = _modalCss;
        document.head.appendChild(s);
    }
}

export function showConfirmModal(onConfirm) {
    ensureModalCss();
    var overlay = document.createElement('div');
    overlay.className = 'modal-overlay';
    overlay.innerHTML =
        '<div class=\'modal-box\'>' +
        '<p class=\'modal-msg\'>' + /*L_RESTART_CONFIRM*/'Do you want to restart the device?' + '</p>' +
        '<div class=\'modal-btn-row\'>' +
        '<button class=\'modal-btn modal-btn-yes\' id=\'mBtnY\'>' + /*L_GENERAL_YES*/'Yes' + '</button>' +
        '<button class=\'modal-btn modal-btn-no\' id=\'mBtnN\'>' + /*L_GENERAL_NO*/'No' + '</button>' +
        '</div></div>';
    document.body.appendChild(overlay);
    document.getElementById('mBtnY').onclick = function() {
        document.body.removeChild(overlay);
        onConfirm();
    };
    document.getElementById('mBtnN').onclick = function() {
        document.body.removeChild(overlay);
    };
}

export function showRestartModal(seconds, message) {
    ensureModalCss();
    var msg = (message !== undefined) ? message : /*L_RESTART_IN_PROGRESS*/'Restart in progress...';
    var div = document.createElement('div');
    div.className = 'modal-overlay';
    div.innerHTML =
        '<div class=\'modal-box\'>' +
        '<div class=\'spinner\'></div>' +
        '<p class=\'modal-msg\'>' + msg + '</p>' +
        '<div class=\'modal-cnt\' id=\'rCnt\'>' + seconds + '</div>' +
        '</div>';
    document.body.appendChild(div);
    var n = seconds;
    var t = setInterval(function () {
        n--;
        var el = document.getElementById('rCnt');
        if (el) el.textContent = n;
        if (n <= 0) { clearInterval(t); location.href = '/'; }
    }, 1000);
}
