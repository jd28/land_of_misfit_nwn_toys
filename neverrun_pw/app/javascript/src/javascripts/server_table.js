/* eslint no-console:0 */

import $ from 'jquery';
import 'datatables.net';
import 'bootstrap';
let format = require('string-format');

import { gametype, version_url, escapeHtml } from "./util";

function copyToClipboard(text, el) {
    let copyTest = document.queryCommandSupported('copy');
    let elOriginalText = el.attr('data-original-title');

    if (copyTest === true) {
        let copyTextArea = document.createElement("textarea");
        copyTextArea.value = text;
        document.body.appendChild(copyTextArea);
        copyTextArea.select();
        try {
            let successful = document.execCommand('copy');
            let msg = successful ? 'Copied!' : 'Whoops, not copied!';
            el.attr('data-original-title', msg).tooltip('show');
        } catch (err) {
            console.log('Oops, unable to copy');
        }
        document.body.removeChild(copyTextArea);
        el.attr('data-original-title', elOriginalText);
    } else {
        // Fallback if browser doesn't support .execCommand('copy')
        window.prompt("Copy to clipboard: Ctrl+C or Command+C, Enter", text);
    }
}

function server_name_status(online, password) {
    if (!online) {
        return 'server-offline';
    } else if (password) {
        return 'server-private';
    } else {
        return 'server-online'
    }
}

function server_name_data(data, type, set, meta) {
    return data;
}

function server_name_render( data, type, row, meta ) {
    let res = `
<div class="server-name {status}">
    <a href="/{version_url}/servers/{id}">{server_name}</a>
</div>
<div class="server-sub">
    <span class="module-name">{module_name}</span>
    <span class="game-type">{type}</span>
</div>
    `;

    return format(res, {
        id: data['id'],
        module_name: escapeHtml(data['module_name']),
        server_name: escapeHtml(data['server_name']),
        status: server_name_status(data['online'], data['private']),
        type: gametype(data['gametype']),
        version_url: version_url(data['version']),
    });
}

function players_data(data, type, set, meta) {
    if(type === "display") {
        return { players: data['players'], players_max: data['players_max']};
    }
    else {
        return data['players'];
    }
}

function players_render(data, type, set, meta) {
    let res =`
<div class="server-count align-right">
{players} / {players_max}
</div>`;

    if(type === "display") {
        return format(res, data);
    }
    else {
        return data;
    }
}

function address_data(data, type, set, meta) {
    return type == 'display' ? { address: data['address'], port: data['port'] } : data['address'];
}

function address_render(data, type, set, meta) {
    let res = `
<div class="server-address-btn btn-grp">
    <span class="server-address">{address}:{port}</span>
    <button class="js-tooltip js-copy" data-toggle="tooltip" data-placement="bottom" data-copy="{address}" title="Copy">
        <span class="far fa-copy"></span>
    </button>
</div>`;

    return type == 'display' ? format(res, data) : data;
}

function createServerTable(v) {
    let table = $('#serverTable').DataTable({
        ajax: {
            "url": "/api/v1/servers/" + version_url(v),
            "dataSrc": ""
        },
        deferRender: true,
        "paging": false,
        "dom": 'tip',
        initComplete: function(settings, json) {
            $('.js-tooltip').tooltip();
            $('.js-copy').click(function() {
                let text = $(this).attr('data-copy');
                let el = $(this);
                copyToClipboard(text, el);
            });
        },
        columns: [
            { data: server_name_data,
                render: server_name_render },
            { data: players_data,
                render: players_render,
                searchable: false,
                type: 'num' },
            { data: address_data,
                render: address_render,
                searchable: false }
        ]
    });

    setInterval(function () {
        table.ajax.reload();
    }, 60 * 1000);

    $('#serverFilterInput').on('change keyup paste', function () {
        console.log($(this).val());
        table.search($(this).val()).draw();
    })
}

function initServerTable(version) {
    $(document).ready(function() {
        createServerTable(version);
    });
}

window.createServerTable = createServerTable;
window.initServerTable = initServerTable;
