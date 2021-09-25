/* eslint no-console:0 */

import $ from 'jquery';
import Chart from 'chart.js';
let format = require('string-format');

function createPlayerChart(data) {
    let labels = []
    for (var i=0; i < data.length; ++i) {
        labels.push(i);
    }

    var playerCountData = {
        labels: labels,
        datasets: [{
            label: "Players",
            backgroundColor: 'rgba(51, 75, 97, 0.75)',
            borderWidth: 1,
            data: data,
            pointHoverRadius: 5,
            pointHoverBackgroundColor: 'rgba(51, 75, 97)'
        }]
    }


    var ctx2 = document.getElementById('playerCountChart').getContext('2d');
    window.playerCountChart = new Chart(ctx2, {
        type: 'line',
        data: playerCountData,
        options: {
            //legend: { display: false },
            maintainAspectRatio: false,
            scales: {
                xAxes: [{ display: false }],
                yAxes: [{ ticks: { beginAtZero:true } }]
            },
            title: {
                display: true,
                position: 'bottom',
                text: 'Last Hour'
            },
        }
    });

}

function createPopularChart(data) {
    var mostPopulartimesChartData = {
        labels: data["labels"],
        datasets: [{
            label: 'Average Players',
            backgroundColor: 'rgba(51, 75, 97, 0.75)',
            borderWidth: 1,
            borderRadius: 4,
            data: data["data"]
        }]
    }

    var ctx = document.getElementById('mostPopularTimesChart').getContext('2d');
    window.mostPopularTimes = new Chart(ctx, {
        type: 'bar',
        data: mostPopulartimesChartData,
        options: {
            maintainAspectRatio: false,
            //legend: { display: false },
            scales: {
                xAxes: [{
                    display: false
                }],
                yAxes: [{
                    ticks: {
                        beginAtZero: true
                    }
                }]
            },
            position: 'bottom',
            title: {
                display: true,
                position: 'bottom',
                text: 'Most Popular Times'
            }
        }
    });
}

function refreshPlayerCharts(v, id) {
    $.ajax({
        url: format('/{version}/servers/{id}/refresh_player_charts', {version: version(v), id: id})
    });
}

function requestPlayerCharts(server_id) {
    $.get('/api/v1/players/'+server_id+'/chart_data', function(data) {
        createPlayerChart(data['players']);
        createPopularChart(data['popular']);
    });
}

window.createPlayerChart = createPlayerChart;
window.createPopularChart = createPopularChart;
window.refreshPlayerCharts = refreshPlayerCharts;
window.requestPlayerCharts = requestPlayerCharts;
