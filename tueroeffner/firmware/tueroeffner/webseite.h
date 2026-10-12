#pragma once

static const char WEBSEITE[] PROGMEM = R"HTML(<!doctype html>
<html lang="de"><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Türöffner</title>
<style>
:root{--bg:#f4f4f2;--fg:#1d1d1b;--karte:#fff;--akzent:#1f6f4a;--fehler:#b3261e;--rand:#ddd}
@media (prefers-color-scheme:dark){:root{--bg:#141414;--fg:#eee;--karte:#1f1f1f;--akzent:#3fae7a;--fehler:#f2827a;--rand:#333}}
*{box-sizing:border-box}body{margin:0;font:17px system-ui,sans-serif;background:var(--bg);color:var(--fg)}
main{max-width:420px;margin:0 auto;padding:24px 16px}
h1{font-size:1.4em;margin:0 0 4px}.klein{opacity:.7;font-size:.9em}
button,input{width:100%;font:inherit;padding:18px;border-radius:14px;border:1px solid var(--rand);margin-top:12px}
input{background:var(--karte);color:var(--fg);text-align:center;letter-spacing:.3em;font-size:1.4em}
button{background:var(--akzent);color:#fff;border:0;font-weight:600}
button.zweit{background:transparent;color:var(--fg);border:1px solid var(--rand);font-weight:400;padding:10px}
button:disabled{opacity:.5}
#meldung{min-height:1.5em;margin-top:14px;text-align:center}.fehler{color:var(--fehler)}
ul{list-style:none;padding:0;margin:20px 0 0}li{padding:6px 0;border-bottom:1px solid var(--rand);font-size:.9em}
[hidden]{display:none}
</style></head><body><main>
<h1>Türöffner</h1><div class="klein" id="hallo">Bitte anmelden</div>

<section id="anmelden" hidden>
  <input id="pin" type="password" inputmode="numeric" autocomplete="current-password" placeholder="PIN">
  <button id="login">Anmelden</button>
</section>

<section id="tueren" hidden>
  <button data-tuer="haus">Haustür öffnen</button>
  <button data-tuer="wohnung">Wohnungstür öffnen</button>
  <ul id="protokoll"></ul>
  <button class="zweit" id="logout">Abmelden</button>
</section>

<div id="meldung"></div>
</main><script>
const $=s=>document.querySelector(s);
function melde(t,f){const m=$('#meldung');m.textContent=t;m.className=f?'fehler':''}
async function post(url,daten){
  const r=await fetch(url,{method:'POST',body:new URLSearchParams(daten||{})});
  let j={};try{j=await r.json()}catch(e){}
  if(!r.ok)throw new Error(j.fehler||('Fehler '+r.status));return j}
async function laden(){
  const s=await (await fetch('/api/status')).json();
  $('#anmelden').hidden=s.angemeldet;$('#tueren').hidden=!s.angemeldet;
  $('#hallo').textContent=s.angemeldet?('Hallo '+s.name):'Bitte anmelden';
  if(s.angemeldet){const ul=$('#protokoll');ul.innerHTML='';
    s.protokoll.forEach(e=>{const li=document.createElement('li');
      li.textContent=e.zeit+'  '+e.name+': '+e.aktion;ul.appendChild(li)})}}
$('#login').onclick=async()=>{try{await post('/api/login',{pin:$('#pin').value});$('#pin').value='';melde('');laden()}catch(e){melde(e.message,1)}};
$('#pin').onkeydown=e=>{if(e.key==='Enter')$('#login').click()};
$('#logout').onclick=async()=>{await post('/api/logout');laden()};
document.querySelectorAll('[data-tuer]').forEach(b=>b.onclick=async()=>{
  b.disabled=true;
  try{await post('/api/oeffnen',{tuer:b.dataset.tuer});
    melde(b.dataset.tuer==='haus'?'Haustür summt, jetzt drücken.':'Klinke unten, Tür aufdrücken.');laden()}
  catch(e){melde(e.message,1)}
  setTimeout(()=>b.disabled=false,3000)});
laden();
</script></body></html>)HTML";
