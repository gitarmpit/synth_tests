document.createElement("article");

var browser = 'ok';

(browser == 'ok')? window.addEventListener('load', init, false):window.attachEvent('onload', init);

var canvas_grid; // canvas raster
var canvas_graph; // canvas graph
var x_grid_size = 300;

x_grid_size = 500;

var y_grid_size = 200;
y_grid_size = 300;
 
var dx_grid = 35;
 
dx_grid = 40;

var dy_grid = 20;
var dy_grid = 25;
var ml = 45;	
var mr = 10;	
var mt = 10;	
var mb = 20;	

var minx = 0;
var stepx = 30;
var maxx = 480;
var miny = -1.2;
var stepy = 0.2;
var maxy = 1.2;

var harmonics = new Array();	// harmonischen array
var phases = new Array();	// Fase array
var harmonics_length = 0;						// harmonischen array lengte
			
var isdrag;			
var dobj;			
var gobj;		

function init() {
	isdrag = false;
	var canvasr = document.getElementById('grid');
	if (canvasr.getContext) {
		canvas_grid = canvasr.getContext("2d");
	}
	var canvasg = document.getElementById('plot');
	if (canvasg.getContext) {
		canvas_graph = canvasg.getContext("2d");
	}
	init_graph();
	draw_grid();
	var i = 0;
	while (document.getElementById('sbutton'+i)) {
		var obj = document.getElementById('sbutton'+i);
		(browser == 'IE')? obj.attachEvent('onmousedown', slidebuttonin) : obj.addEventListener('mousedown', slidebuttonin, false);
		obj.addEventListener('touchstart', slidebuttonin, false);
		i++;
	}
	
	var obj = document.getElementById('webapi').getElementsByTagName('button');
	if (obj.length > 0) {
		obj[0].addEventListener('click', function(e) {freqSound(e,0);}, false);
		obj[0].addEventListener('touchstart', function(e) {freqSound(e,0);}, false);
		obj[1].addEventListener('click', function(e) {freqSound(e,1);}, false);
		obj[1].addEventListener('touchstart', function(e) {freqSound(e,1);}, false);
		obj[2].addEventListener('click', function(e) {amplSound(e,0);}, false);
		obj[2].addEventListener('touchstart', function(e) {amplSound(e,0);}, false);
		obj[3].addEventListener('click', function(e) {amplSound(e,1);}, false);
		obj[3].addEventListener('touchstart', function(e) {amplSound(e,1);}, false);
		document.getElementById('freqval').addEventListener('change', freqInput, false);
	}
	
	if ('AudioContext' in window) {
		context = new AudioContext();
		setupSound();
	}
	else if ('webkitAudioContext' in window) {
		context = new webkitAudioContext();
		setupSound();
	}
	else {
		context = false;
		document.getElementById('webapi').innerHTML =  '<h3>Sound</h3><p>Your browser is not capable of processing<br><b>Web Audio API</b>.</p>';
	}
	
	set_harmonics('sinus');
}


function set_sin(obj,ha) 
{
	if (obj.innerHTML == 'sin') 
        {
		obj.innerHTML = 'cos';
		phases[ha] += Math.PI/2;
	}
	else 
        {
		obj.innerHTML = 'sin';
		phases[ha] -= Math.PI/2;
	}
	graph_signal();
	updateSoundWave();
}

/*** Set pos/neg ***/
function set_pol(obj,ha) 
{
	if (dit.innerHTML == '+') 
        {
		dit.innerHTML = '-';
		phases[ha] += Math.PI;
	}
	else 
        {
		dit.innerHTML = '+';
		phases[ha] -= Math.PI;
	}
	graph_signal();
	updateSoundWave();
}

/*** Positioneer schuiven en vul values vanuit array ***/
function set_schuifveld() {
	var i = 0;
	while (document.getElementById('sbutton'+i)) {
		var ks = (i==0) ? 0.5 : 1;
		var obj = document.getElementById('sbutton'+i);
		var spos = obj.parentNode.offsetHeight - harmonics[i] * obj.parentNode.offsetHeight*ks;
		obj.style.top = spos-7+'px';
		document.getElementById('sw'+i).innerHTML = harmonics[i].toFixed(3);
		if (i > 0) 
                {
			var spb = document.getElementById('sh'+i).getElementsByTagName('button');
			var tmp = phases[i];
			if (tmp > Math.PI*0.9) 
                        {
			   tmp -= Math.PI; 
                           spb[1].innerHTML = '-';
                        }
			else 
                        {
                            spb[1].innerHTML = '+';
                        }
			if (tmp > Math.PI*0.45) 
                        {
			   spb[0].innerHTML = 'cos';
                        }
			else 
                        {
                           spb[0].innerHTML = 'sin';
                        }
		}

		i++;
	}
}


/*** Op een slidebutton geklikt ***/
function slidebuttonin(e) {
	e.preventDefault();
	var fobj = (browser == 'ok')? e.target : event.srcElement;
	if (fobj.className == 'slidebutton') {
		isdrag = true;
		dobj = fobj;
		gobj = dobj.parentNode.parentNode;
		(browser == 'IE')? gobj.attachEvent('onmouseup', slidebuttonlos) : gobj.addEventListener('mouseup', slidebuttonlos, false);
		gobj.addEventListener('touchend', slidebuttonlos, false);
		(browser == 'IE')? gobj.attachEvent('onmouseleave', slidebuttonlos) : gobj.addEventListener('mouseleave', slidebuttonlos, false);
		gobj.addEventListener('touchleave', slidebuttonlos, false);
		(browser == 'IE')? gobj.attachEvent('onmousemove', slidebuttonslepen) : gobj.addEventListener('mousemove', slidebuttonslepen, false);
		gobj.addEventListener('touchmove', slidebuttonslepen, false);
		dobj.style.borderColor = '#F00';
	}
}


function slidebuttonlos(e) {
	if (gobj.className == 'slider') {
		isdrag = false;
		(browser == 'IE')? gobj.detachEvent('onmouseup', slidebuttonlos) : gobj.removeEventListener('mouseup', slidebuttonlos, false);
		gobj.removeEventListener('touchend', slidebuttonlos, false);
		(browser == 'IE')? gobj.detachEvent('onmouseleave', slidebuttonlos) : gobj.removeEventListener('mouseleave', slidebuttonlos, false);
		gobj.removeEventListener('touchleave', slidebuttonlos, false);
		(browser == 'IE')? gobj.detachEvent('onmousemove', slidebuttonslepen) : gobj.removeEventListener('mousemove', slidebuttonslepen, false);
		gobj.removeEventListener('touchmove', slidebuttonslepen, false);
		dobj.style.borderColor = '#88A';
	}
}

function slidebuttonslepen(e) 
{
	if (isdrag) {
		e.preventDefault();
		var ysl = (browser == 'ok')? e.pageY : event.pageY;
		var scrollbar = dobj.parentNode;
		var yval = ysl - scrollbar.offsetTop;
		if (yval < 0) {yval = 0;}
		if (yval > scrollbar.offsetHeight) {yval = scrollbar.offsetHeight;}
		dobj.style.top = yval-7+'px';
		var pros = (scrollbar.offsetHeight - yval) / scrollbar.offsetHeight;
		var i = parseInt(dobj.id.replace(/[^0-9]/g,''));
		var pros = (i==0) ? pros*2 : pros*1;
		pros = pros.toFixed(3);
		harmonics[i] = pros;
		document.getElementById('sw'+i).innerHTML = pros;
		graph_signal();
		updateSoundWave();
	}
}


function graph_signal() {

	canvas_graph.clearRect(0, 0, x_grid_size+ml+mr, y_grid_size+mt+mb);
	canvas_graph.beginPath();
	canvas_graph.strokeStyle = '#00F';
	var ys = harmonics[0]*y_grid_size/(maxy-miny);
	var yo = y_grid_size/2;
	var xs = (maxx-minx)*2*Math.PI/(x_grid_size*360);
	var xo = 0;
	var yval;
	var mulx;
	for (var i=0; i<=x_grid_size; i++) 
        {
		yval = 0;
		mulx = i*xs;
		for (var ha=1; ha<harmonics_length; ha++) 
                {
			yval += harmonics[ha]*Math.sin((ha*mulx)+phases[ha]);
		}
		yval = yo - yval*ys;
		
		if (i == 0) 
                {
		  canvas_graph.moveTo(i+ml, yval+mt);
                }
		else 
                {
                  canvas_graph.lineTo(i+ml, yval+mt);
                }
	}

	canvas_graph.stroke();
}


function set_harmonics(signal) {
	var pi = Math.PI;
	switch(signal) {
		case 'sinus' :
			harmonics = new Array(1,1,0,0,0,0,0,0,0,0,0,0);
			phases = new Array(0,0,0,0,0,0,0,0,0,0,0,0);
			break;
		case 'blok' :
			harmonics = new Array(4/pi,1,0,0.3333,0,0.2,0,0.1429,0,0.1111,0,0.0909);
			phases = new Array(0,0,0,0,0,0,0,0,0,0,0,0);
			break;
		case 'trapezium' :
			harmonics = new Array(4/pi,0.993,0,0.314,0,0.168,0,0.101,0,0.06,0,0.033);
			phases = new Array(0,0,0,0,0,0,0,0,0,0,0,0);
			break;
		case 'driehoek' :
			harmonics = new Array(0.81,1,0,0.11111,0,0.04,0,0.02041,0,0.0123,0,0.0083);
			phases = new Array(0,pi/2,0,pi/2,0,pi/2,0,pi/2,0,pi/2,0,pi/2);
			break;
		case 'zaagtand' :
			harmonics = new Array(2/pi,1,0.5,0.333,0.25,0.2,0.1667,0.1429,0.125,0.1111,0.1,0.0909);
			phases = new Array(0,0,0,0,0,0,0,0,0,0,0,0);
			break;
		case 'impuls' :
			harmonics = new Array(0.18,1,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0);
			phases = new Array(0,0,pi*1.5,pi,pi/2,0,pi*1.5,pi,pi/2,0,pi*1.5,pi);
			break;
		case 'viool' :
			harmonics = new Array(0.49,0.995,0.94,0.425,0.480,0,0.365,0.04,0.085,0,0.09,0);
			phases = new Array(0,0,pi/2,0,pi/2,0,pi/2,0,pi/2,0,pi/2,0);
			break;
	}
	harmonics_length = harmonics.length;
	set_schuifveld();
	graph_signal();
	updateSoundWave();
}


function init_graph() {
	x_grid_size = dx_grid*(maxx-minx)/stepx;
	y_grid_size = dy_grid*(maxy-miny)/stepy;
	var xveld = x_grid_size+ml+mr;
	var yveld = y_grid_size+mt+mb;
	canvas_grid.canvas.width = xveld;
	canvas_grid.canvas.height = yveld;
	canvas_graph.canvas.width = xveld;
	canvas_graph.canvas.height = yveld;
	canvas_grid.clearRect(0, 0, xveld, yveld);
	var obj = document.getElementById('graph');
	obj.style.width = xveld + 'px';
	obj.style.height = yveld + 'px';
}

function draw_grid() {
	canvas_grid.strokeStyle = '#888';
	canvas_grid.lineWidth = 1;
	canvas_grid.font = '8pt verdana';
	canvas_grid.textBaseline = 'middle';
	var lab;
	// Horizontale grid lines
	canvas_grid.textAlign = 'right';
	var lw = maxy;
	var dy = y_grid_size/10;
	for (var yi=0; yi<=y_grid_size; yi+=dy_grid) {
		canvas_grid.beginPath();
		canvas_grid.moveTo(0+ml-5, yi+mt);
		canvas_grid.lineTo(x_grid_size+ml, yi+mt);
		canvas_grid.stroke();
		lab = make_labels(lw, 3);
		canvas_grid.fillText(lab, ml-8, mt+yi);
		lw -= stepy;
	}

	// Verticale grid lines
	canvas_grid.textAlign = 'center';
	var lw = minx;
	for (var xi=0; xi<=x_grid_size; xi+=dx_grid) {
		canvas_grid.beginPath();
		canvas_grid.moveTo(xi+ml, 0+mt);
		canvas_grid.lineTo(xi+ml, y_grid_size+mt+5);
		canvas_grid.stroke();
		lab = make_labels(lw, 3);
		canvas_grid.fillText(lab, ml+xi, mt+y_grid_size+12);
		lw += stepx;
	}

	var gsx = x_grid_size / (maxx - minx) * (360 - minx);
	canvas_grid.fillStyle = 'rgba(100,0,0,0.3)';
	canvas_grid.fillRect(gsx + ml, mt, x_grid_size - gsx,y_grid_size);
}


function make_labels(value, deci) {
	var prefix = new Array('a','f','p','n','µ','m','','k','M','G','T','P','E');
	var pp = 6;
	var sgn = '';
	if (value > -1e-15 && value < 1e-15) {
		value = 0;
	}
	else {
		if (value < 0) {
			sgn = '-';
			value = Math.abs(value);
		}
		while (value >= 1000 && prefix[pp+1]) {
			value /= 1000;
			pp++;
		}
		while (value < 1 && prefix[pp-1]) {
			value *= 1000;
			pp--;
		}
		var sh = 0;
		if (value >= 100) {
			value /= 100;
			sh = 2;
		}
		else if (value >= 10) {
			value /= 10;
			sh = 1;
		}
		value = Math.round(Math.pow(10,deci)*value);
		value /= Math.pow(10,deci-sh);
	}
	return sgn+value+prefix[pp];
}

/*** Web Audio API ***/

var context;
var oscillator;
var volume;
var note = 0;
var ampl = 0;
var playing = false;
var audioRuns = false;	// in Chrome the user has to start the audio manually

function calcFreq() {
  return Math.pow(2, note / 12)*440;
}

function setupSound() {
	oscillator = context.createOscillator();
	oscillator.frequency.value = calcFreq();
	//volume = context.createGainNode(); // old
	volume = context.createGain(); // new
	volume.gain.value = ampl;
	oscillator.connect(volume);
	volume.connect(context.destination);
	document.getElementById('freqval').value = calcFreq();
	audioRuns = false;
	playing = true;
	oscillator.start(0);
	//oscillator.noteOn(0);
}

function freqInput(e) 
{
	var obj = (browser == 'IE')? event.srcElement : e.target;
	var freq = obj.value;
	if (freq < 1) 
        {
           freq = 1;
        }
	
        if (freq > 20000) 
        {
          freq = 20000;
        }

	oscillator.frequency.value = freq;
	document.getElementById('freqval').value = freq;
	note = Math.round((Math.log(freq / 440)/Math.log(2)) * 12);
}

function freqSound(e,dir) 
{
	e.preventDefault();
	if (context !== false) 
        {
          if (dir == 0) 
          {
	     note--; 
          }
	  else 
          {
	     note++; 
          }
          var freq = calcFreq(note);
          oscillator.frequency.value = freq;
          document.getElementById('freqval').value = freq.toFixed(1);
	}
}

function amplSound(e,dir) 
{
	e.preventDefault();
	if (context !== false) 
        {
          if (dir == 0) 
          {
             ampl -= 0.1;
	     if (ampl < 0) {ampl = 0;} 
	  }
	  else 
          {
            if (audioRuns === false) 
            {
               context.resume(); 
               audioRuns = true;
            }
			
            ampl += 0.1; 
	    if (ampl > 1) 
            {
               ampl = 1;
            }
          }
	  ampl = Math.round(ampl * 10) / 10;
	  volume.gain.value = ampl;
	  var obj = document.getElementById('amplval');
	  document.getElementById('amplvalbar').style.width = parseInt(obj.offsetWidth * ampl)+'px';
	  var nobj = document.getElementById('amplvalnum');
	  if (Math.round(ampl * 10) == 0) 
	  {
	     nobj.innerHTML = 'off';
          }
	  else 
          {
	     obj.innerHTML = ampl;
          }
	}
}


function updateSoundWave() 
{
  if (context !== false) 
  {
	var pi = Math.PI;
	var nHarm = 11+1;
	var curveSin = new Float32Array(nHarm);
	var curveCos = new Float32Array(nHarm);
	curveSin[0] = 0;
	curveCos[0] = 0;
	for (var i = 1; i < nHarm; i++) 
	{
		curveSin[i] = harmonics[i] * harmonics[0] * Math.cos(phases[i]);
		curveCos[i] = harmonics[i] * harmonics[0] * Math.sin(phases[i]);
	}		
	var waveTable = context.createPeriodicWave(curveCos, curveSin, {disableNormalization: true});
	oscillator.setPeriodicWave(waveTable);
  }
}


