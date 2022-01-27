var canvas;
var ctx;
var shiftDown = false;
var mouseDown = false;
var screen_w = 0;
var screen_h = 0;
var margin_left = 2;
var margin_right = 2;
var margin_top = 2;
var margin_bottom = 2;
var graph_w = 0;
var graph_h = 0;

var mouse_x = 0;
var mouse_y = 0;
var center_x = 0;
var center_y = 0;
var graph_x0 = margin_left;
var graph_y0 = margin_right;

var range_x = 20;
var range_y = 20;

var minor_grid_step_def = 18;
var minor_grid_step_min = 15;
var minor_grid_step_max = 28;
var minor_grid_step_x = minor_grid_step_def;
var minor_grid_step_y = minor_grid_step_def;
var minor_grid_per_major = 5;

var total_minor_grids_x = 0;
var total_major_grids_x = 0;

var total_minor_grids_y = 0;
var total_major_grids_y = 0;

var l1, l2, l3, l4;

var zoomx = 0;
var zoomy = 0;

var resize_h = false;
var resize_v = false;

var move_cnt = 0;

function init() {
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');
    window.addEventListener('resize', resizeCanvas, false);
    document.addEventListener('keyup', keyup);
    document.addEventListener('keydown', keydown);
    canvas.addEventListener('mousemove', mousemove, true);
    canvas.addEventListener("wheel", wheel, true);
    canvas.addEventListener("mousedown", mousedown, true);
    canvas.addEventListener("mouseup", mouseup, true);
    canvas.addEventListener('contextmenu', rightClick, true);

    l1 = document.getElementById('l1');
    l2 = document.getElementById('l2');
    l3 = document.getElementById('l3');
    l4 = document.getElementById('l4');

    set_geometry();
    resetCenter();
    resizeCanvas();

    l1.innerHTML = zoomx;
    l2.innerHTML = zoomy;


}

function resetCenter() {
    center_x = Math.floor((graph_x0 + graph_w / 2) / minor_grid_step_x);
    center_y = Math.floor((graph_y0 + graph_h / 2) / minor_grid_step_y);
}

function set_geometry() {
    var labelDiv = document.getElementById('labelDiv');
    screen_w = window.innerWidth;
    screen_h = window.innerHeight - labelDiv.clientHeight;

    canvas.width = screen_w;
    canvas.height = screen_w;
    graph_w = screen_w - margin_left - margin_right;
    graph_h = screen_h - margin_top - margin_bottom;
    graph_x0 = margin_left;
    graph_y0 = margin_top;

    //calculate grids 
    total_minor_grids_x = Math.floor(graph_w / minor_grid_step_x);
    total_major_grids_x = Math.floor(total_minor_grids_x / minor_grid_per_major);
    total_minor_grids_y = Math.floor(graph_h / minor_grid_step_y);
    total_major_grids_y = Math.floor(total_minor_grids_y / minor_grid_per_major);

}

function resizeCanvas() {
    var old_graph_w = graph_w;
    var old_graph_h = graph_h;
    set_geometry();
    center_x = Math.round(center_x * minor_grid_step_x / old_graph_w * graph_w / minor_grid_step_x);
    center_y = Math.round(center_y * minor_grid_step_y / old_graph_h * graph_h / minor_grid_step_y);
    drawAxis();
    //redraw();
}

function redraw() {
    // canvas border
    ctx.clearRect(0, 0, window.innerWidth, window.innerHeight);
    ctx.strokeStyle = 'blue';
    ctx.lineWidth = '2';
    ctx.strokeRect(0, 0, window.innerWidth, window.innerHeight);
    drawAxis();
}

function drawLine(x0, y0, x1, y1) {
    ctx.beginPath();
    ctx.moveTo(x0, y0);
    ctx.lineTo(x1, y1);
    ctx.stroke();
    ctx.closePath();
}

function drawAxis() {

    // graph border
    ctx.clearRect(graph_x0, graph_y0, graph_w, graph_h);
    ctx.strokeStyle = "rgba(0, 120, 60, 1)";
    ctx.lineWidth = '1';
    ctx.strokeRect(graph_x0, graph_y0, graph_w, graph_h);

    //ctx.fillStyle = "rgba(0, 0, 0, 0)";
    //ctx.fillRect(0, 0, window.innerWidth, window.innerHeight);

    // Minor axis
    var minorStyle = '#dedede';
    var majorStyle = '#777777'
    ctx.strokeStyle = minorStyle;
    ctx.lineWidth = '1';

    // X-axis minor
    for (i = 0; i < total_minor_grids_y + 2; ++i) {
        if (((total_minor_grids_y*2 - i) % minor_grid_per_major) == 0) {
            ctx.beginPath();
            ctx.strokeStyle = minorStyle;
            console.log("i = " + i + ": " + (total_minor_grids_y * 2 - i) % minor_grid_per_major);
        }
        else {
            ctx.beginPath();
            ctx.strokeStyle = minorStyle;
        }
        drawLine(
            graph_x0,
            graph_y0 + minor_grid_step_y * (i) + 0.5,
            graph_x0 + graph_w,
            graph_y0 + minor_grid_step_y * (i) + 0.5);
    }
    ctx.strokeStyle = minorStyle;
    ctx.lineWidth = '1';
    ctx.beginPath();

    // Y-axis minor
    for (i = 0; i < total_minor_grids_x + 2; ++i) {
        drawLine(
            graph_x0 + minor_grid_step_x * (i) + 0.5,
            graph_y0,
            graph_x0 + minor_grid_step_x * (i) + 0.5,
            graph_y0 + graph_h);
    }

    console.log("total minor: " + total_minor_grids_y);

    // Main axis

    //ctx.strokeStyle = '#969696';
    ctx.strokeStyle = 'black';
    ctx.lineWidth = '1';
    // ctx.strokeStyle = "rgb(140,140,140)";

    // X-axis
    drawLine(graph_x0, center_y * minor_grid_step_y + 0.5,
        graph_x0 + graph_w, center_y * minor_grid_step_y + 0.5);
 
    // Y-axis
    drawLine(center_x * minor_grid_step_x + 0.5, graph_y0,
        center_x * minor_grid_step_x+ 0.5, graph_y0 + graph_h);
 
}

function change_zoom_x(inc) {
    if (inc) {
        ++zoomx;
        if (minor_grid_step_x < minor_grid_step_max) {
            ++minor_grid_step_x;
        }
        else {
            //minor_grid_step_x = minor_grid_step_def;
        }
    }
    else {
        --zoomx;
        if (minor_grid_step_x > minor_grid_step_min) {
            --minor_grid_step_x;
        }
        else {
            //minor_grid_step_x = minor_grid_step_max;
        }
    }
}

function change_zoom_y(inc) {
    if (inc) {
        ++zoomy;
        if (minor_grid_step_y < minor_grid_step_max) {
            ++minor_grid_step_y;
        }
        else {
            //minor_grid_step_y = minor_grid_step_def;
        }
    }
    else {
        --zoomy;
        if (minor_grid_step_y > minor_grid_step_min) {
            --minor_grid_step_y;
        }
        else {
            //minor_grid_step_y = minor_grid_step_max;
        }
    }
}

function keyup(event) {
    if (event.which || event.keyCode === 16) {
        event.preventDefault();
        shiftDown = false;
        canvas.style.cursor = "default";
        resize_h = false;
        resize_v = false;
    }
}

function keydown(event) {
    if (!shiftDown && event.shiftKey) {
        event.preventDefault();
        shiftDown = true;
        change_cursor();
    }

}


function change_cursor() {
    if (Math.abs(mouse_x - center_x) < 20) {
        canvas.style.cursor = "n-resize";
        resize_h = false;
        resize_v = true;
    }
    else if (Math.abs(mouse_y - center_y) < 20) {
        canvas.style.cursor = "e-resize";
        resize_h = true;
        resize_v = false;
    }
    else {
        canvas.style.cursor = "ne-resize";
        resize_h = true;
        resize_v = true;
    }
}

function mousemove(event) {
    var dx = event.movementX;
    var dy = event.movementY;
    mouse_x = event.clientX;
    mouse_y = event.clientY;
    l3.innerHTML = move_cnt++;

    if (shiftDown) {
        if (mouseDown) {
            if (resize_v && resize_h) {
                var inc = (dy < 0) ? true : false;
                change_zoom_x(inc);
                change_zoom_y(inc);
            }
            else if (resize_h) {
                var inc = (dx < 0) ? true : false;
                if (center_x < mouse_x) {
                    inc = !inc;
                }
                change_zoom_x(inc);
            }
            else if (resize_v) {
                var inc = (dy < 0) ? true : false;
                if (center_y < mouse_y) {
                    inc = !inc;
                }
                change_zoom_y(inc);
            }
        }
        else {
            change_cursor();
        }

        l1.innerHTML = zoomx;
        l2.innerHTML = zoomy;
        set_geometry();
        drawAxis();

    }
    else if (mouseDown) {
        if ((center_x + dx) > graph_x0 &&
            (center_x + dx) < (graph_x0 + graph_w) &&
            (center_y + dy) > graph_y0 &&
            (center_y + dy) < (graph_y0 + graph_h)) {
            center_x += dx;
            center_y += dy;
            set_geometry();
            drawAxis();
        }
    }
}

// zoom
function wheel(event) {
    if (shiftDown) {
        if (resize_h && !resize_v) {
            //var inc = (center_x > mouse_x) ? true : false;
            if (event.deltaY < 0) {
                change_zoom_x(true);
            }
            else {
                change_zoom_x(false);
            }
        }
        else if (resize_v && !resize_h) {
            if (event.deltaY < 0) {
                change_zoom_y(true);
            }
            else {
                change_zoom_y(false);
            }
        }
        else if (event.deltaY < 0) {
            change_zoom_x(true);
            change_zoom_y(true);
        }
        else {
            change_zoom_x(false);
            change_zoom_y(false);
        }
    }
    else {
        if (event.deltaY < 0) {
            change_zoom_x(true);
            change_zoom_y(true);
        }
        else if (event.deltaY > 0) {
            change_zoom_x(false);
            change_zoom_y(false);
        }
    }

    l1.innerHTML = zoomx;
    l2.innerHTML = zoomy;
    set_geometry();
    drawAxis();
}

function mousedown(event) {
    event.preventDefault();
    if (event.button === 0) {
        mouse_x = event.x;
        mouse_y = event.y;
        mouseDown = true;
    }
}

function mouseup(event) {
    if (event.button === 0) {
        mouseDown = false;
    }
}

function rightClick(event) {
    event.preventDefault();
    resetCenter();
    minor_grid_step_x = minor_grid_step_def;
    minor_grid_step_y = minor_grid_step_def;
    set_geometry();
    drawAxis();
    zoomx = 0;
    zoomy = 0;
    l1.innerHTML = zoomx;
    l2.innerHTML = zoomy;
}

