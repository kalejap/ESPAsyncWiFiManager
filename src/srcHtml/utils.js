/**
 * Utility functions for DOM manipulation and simple logic.
 */

// Shorthand for document.getElementById
const gebi=s=>document.getElementById(s);

const qs=s=>document.querySelector(s);

const qsa=s=>document.querySelectorAll(s);

const wl=f=>window.addEventListener('load',f);

// UI helper: set inner element
const IH=(l,v)=>{
    gebi(l).textContent=v
};

/**
 * Creates a complete HTML Table element (<table>) from an array of JSON objects.
 * * @param {Array<Object>} dt An array of objects, where each object is a table row.
 * @param {Object} [opt] Optional configuration settings.
 * @param {Object} [opt.rowHeadLocals] An object map where keys match dt keys 
 * (e.g., { 'id': 'Employee ID' }). If a key is missing, the original key is used.
 * @param {boolean} [opt.colHeadLocals=null] An object map where keys match dt keys 
 * (e.g., { 'id': 'Employee ID' }). If a key is missing, the original key is used.
 * @param {string} [opt.tableClass] An optional CSS class to be added to the <table> element.
 * @returns {HTMLTableElement | null} A fully constructed <table> element or null 
 * if dt is invalid or empty.
 */
function j2t(dt, opt = {}) {
    // 1. Basic validation
    if (!Array.isArray(dt) || dt.length === 0 || typeof dt[0] !== 'object') {
        console.warn('jsonToTable requires a non-empty array of objects.');
        return null;
    }

    const tbl = document.createElement('table'); 
    
    // Add optional class
    if (opt.tableClass) {
        tbl.classList.add(opt.tableClass); 
    }
    
    // Get column headers (keys) from the first object
    const colHds = Object.keys(dt[0]); 
    
    const firstColIsHeader = opt.firstColIsHeader === true;

    if (opt.rowHeadLocals)
    {
        // Create Table Header (<thead>)
        const tHead = tbl.createTHead(); 
        const hRow = tHead.insertRow(); 
    
        colHds.forEach(key => {
            const th = document.createElement('th');
            
            // Use localized header text if provided, otherwise use the original key.
            let hdrTxt = opt.rowHeadLocals[key] || key;
            th.textContent = hdrTxt;
            
            hRow.appendChild(th);
        });
    }

    // 3. Create Table Body (<tbody>)
    const tBody = tbl.createTBody();

    dt.forEach(dRow => { 
        const row = tBody.insertRow();
        let rowIdentity = ''; // This will store the value of the first column
        
        colHds.forEach((key, index) => {
            let val = dRow[key];
            let cellContent = val !== null && val !== undefined ? val.toString() : '';
            let cell;

            if (index === 0) {
                // 1. Identify the row based on the first column's value
                if (opt.colHeadLocals) {
                    rowIdentity = opt.colHeadLocals[key] || key;
                } else {
                    rowIdentity = cellContent;
                }

                // Create the first cell (often a TH scope='row')
                cell = document.createElement('th');
                cell.setAttribute('scope', 'row');
                cell.textContent = rowIdentity;
            } else {
                // 2. Create standard data cells
                cell = row.insertCell();
                cell.textContent = cellContent;

                // 3. Set the data-label to the first column's value 
                // if rowHeadLocals isn't provided
                const mobileLabel = (opt.rowHeadLocals && opt.rowHeadLocals[key]) 
                                    ? opt.rowHeadLocals[key] 
                                    : rowIdentity;
                
                cell.setAttribute('data-label', mobileLabel);
            }
            
            row.appendChild(cell);
        });
    });
    
    return tbl;
}

/**
 * (JSON to form) Populates HTML input elements with values from a JSON object.
 * The key of the JSON object must match the 'id' of the HTML element.
 *
 * @param {Object} data - A JSON object where keys are element IDs and values are the input values.
 */
function j2f(data) {
  // Check if the input is a valid object
  if (typeof data !== 'object' || data === null) {
    console.error('Input data must be a non-null object.');
    return;
  }

  // Iterate over each key-value pair in the JSON object
  for (const elementId in data) {
    if (Object.prototype.hasOwnProperty.call(data, elementId)) {
      const valueToSet = data[elementId];

      // 1. Get the HTML element by its ID
      const element = gebi(elementId);

      // 2. Check if the element exists
      if (element) {
        // 3. Set the 'value' property of the input element
        element.value = valueToSet;
      } else {
        //console.warn(`Element with ID '${elementId}' not found in the DOM.`);
      }
    }
  }
}

/**
 * Fetches JSON data from a server, converts it to an HTML table,
 * and inserts the table into a target element.
 * * @param {string} url The endpoint to fetch the JSON data from.
 * @param {string} targetElementId The ID of the HTML element where the table will be inserted.
 */
async function u2t(url, id, opt = {}) {
    const el = gebi(id);
    
    // Clear the target area and show a loading message
    el.innerHTML = 'Fetching data...';

    if (!el) {
        console.error(`Target element with ID '${id}' not found.`);
        return;
    }

    try {
        // 1. Fetch Data
        const response = await mfetch(url);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        // 2. Parse JSON
        // The data is expected to be an array of objects, e.g., [{'ID': 101, 'Name': 'Alpha'}, ...]
        const data = await response.json(); 

        if (data.length === 0) {
            el.innerHTML = 'No data available.';
            return;
        }

        // Convert JSON to table
        const table = j2t(data, opt);

        // 5. Insert Table into DOM
        // Instead of setting innerHTML, we replace the content with the new DOM node.
        el.innerHTML = ''; // Clear loading message
        el.appendChild(table);

    } catch (error) {
        console.error('Failed to load or convert data:', error);
        el.textContent = `Error: ${error.message}`;
    }
}

/**
 * Fetches JSON data from a server, converts it to an HTML table,
 * and inserts the table into a target element.
 * * @param {string} url The endpoint to fetch the JSON data from.
 * @param {string} targetElementId The ID of the HTML element where the table will be inserted.
 */
async function u2f(url) {
    try {
        // 1. Fetch Data
        const response = await mfetch(url);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        // 2. Parse JSON
        // The data is expected to be an array of objects, e.g., [{'ID': 101, 'Name': 'Alpha'}, ...]
        const data = await response.json(); 

        j2f(data);
    } catch (error) {
        console.error('Failed to load or convert data:', error);
    }
}

async function mfetch(url) {
    return fetch(url);
}

// Export all utility functions for use in main.js
export { gebi, qs, qsa, wl, IH, j2t, u2t, j2f, u2f };