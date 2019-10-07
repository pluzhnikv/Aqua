const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="ru">
<head>
    <style>
        * {
            box-sizing: border-box
        }

        body, html {
            height: 100%;
            margin: 0;
            font-family: Arial, serif;
            text-align: center;
        }

        .tabs-container {
            display: flex;
            justify-content: space-between;
            align-items: stretch;
            text-align: center;
            min-height: 48px;
            background-color: #1976d2;
            color: #fff;
            box-shadow: 0 2px 4px -1px rgba(0, 0, 0, 0.2), 0 4px 5px 0 rgba(0, 0, 0, 0.14), 0 1px 10px 0 rgba(0, 0, 0, 0.12);
        }

        .tab {
            flex: 1;
            opacity: 0.7;
            cursor: pointer;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .tab.active {
            opacity: 1;
            border-bottom: 2px solid #dc004e;
        }

        .tab-content > div {
            display: none;
            margin: 10px;
        }

        .tab-content > div.active {
            display: block;
        }

        .server-text-wrapper {

        }
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
    <title>Aquarium management</title>
</head>
<body>

<div class="tabs-container">
    <div class="tab active" onclick="openPage('control', this)"><span>Управление</span></div>
    <div class="tab" onclick="openPage('params', this)"><span>Параметры</span></div>
    <div class="tab" onclick="openPage('about', this)"><span>О программе</span></div>
</div>

<div class="tab-content">
    <div id="control" class="active">
        <h4>Светодиодные лампы:</h4>
        <div>
            <input type="range" min="0" max="100" class="slider" id="wc">
            <input type="range" min="0" max="100" class="slider" id="ww">
            <input type="range" min="0" max="100" class="slider" id="r">
            <input type="range" min="0" max="100" class="slider" id="g">
            <input type="range" min="0" max="100" class="slider" id="b">
            <input type="range" min="0" max="100" class="slider" id="uv">

            <div class="server-text-wrapper">Server response: <span id="serverText">NA</span></div>
        </div>
    </div>

    <div id="params">
        <h4>Здесь будут параметры аквариума</h4>
    </div>

    <div id="about">
        <h4>Who we are and what we do.</h4>
    </div>
</div>


<script>
  const serverText = document.getElementById('serverText');
  let oldParams = getInputsValuesAsQueryParams();

  getParams(); // Get initial params from server
  // let getParamsTimerId = setInterval(getParams, 5e3); // Get params from server every 5 seconds.

  let setLampsTimerId = setInterval(setLamps, 200); // Get params from server every 5 seconds.

  function getInputs() {
    return [...document.querySelectorAll('input[type=range]')];
  }

  function getInputsValuesAsQueryParams() {
    return getInputs().reduce((params, input) => params + `${input.id}=${input.value}&`, '?').slice(0, -1);
  }

  async function getParams() {
    const response = await fetch('getParams');
    if (!response.ok) alert('HTTP error: ' + response.status); // HTTP status is not in range 200-299

    let responseText = await response.text();
    const responseValues = responseText.split(' ');
    getInputs().forEach((input, index) => input.value = responseValues[index]);
    serverText.innerHTML = responseText;
  }

  async function setLamps() {
    const newParams = getInputsValuesAsQueryParams();
    if (oldParams === newParams) return;
    oldParams = newParams;

    const response = await fetch(`setLamps${newParams}`);
    if (!response.ok) alert('HTTP error: ' + response.status); // HTTP status is not in range 200-299

    serverText.innerHTML = await response.text();
  }

  function openPage(pageName, element) {
    const tabs = document.querySelectorAll('.tab');
    tabs.forEach((tab) => tab.classList.remove('active'));
    element.classList.add('active');

    const activeTabContent = document.getElementById(pageName);
    const allTabsContent = document.querySelectorAll(`.tab-content > div`);
    allTabsContent.forEach((tabContent) => tabContent.classList.remove('active'));
    activeTabContent.classList.add('active');
  }
</script>
</body>
</html>
)=====";
