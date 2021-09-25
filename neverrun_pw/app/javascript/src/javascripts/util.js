/* eslint no-console:0 */

export function escapeHtml(text) {
    return text
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}

export function gametype(type) {
    let res = 'Unkwnown';
    switch (type) {
        case 0: res = 'Action'; break;
        case 1: res = 'Story'; break;
        case 2: res = 'Story Lite'; break;
        case 3: res = 'Role Play'; break;
        case 4: res = 'Team'; break;
        case 5: res = 'Melee'; break;
        case 6: res = 'Arena'; break;
        case 7: res = 'Social'; break;
        case 8: res = 'Alternative'; break;
        case 9: res = 'PW Action'; break;
        case 10: res = 'PW Story'; break;
        case 11: res = 'Solo'; break;
        case 12: res = 'Tech Support'; break;
    }
    return res;
}

export function version_url(v) {
    switch(v) {
        case 1: return 'nwn1';
        case 2: return 'nwn2';
        case 3: return 'nwnee';
        default: return 'unknown'
    }
}
