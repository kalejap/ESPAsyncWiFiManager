// server.js
const http = require('http');
const fs = require('fs');
const path = require('path');

// Configuration
const PORT = 3000;
const PUBLIC_DIR = path.join(__dirname, '/'); // Serve files from the current directory

// Function to map file extensions to MIME types
const getContentType = (filePath) => {
    const extname = path.extname(filePath);
    switch (extname) {
        case '.html':
            return 'text/html';
        case '.css':
            return 'text/css';
        case '.js':
            return 'text/javascript';
        case '.json':
            return 'application/json';
        case '.png':
            return 'image/png';
        case '.jpg':
            return 'image/jpg';
        default:
            return 'application/octet-stream';
    }
};

const server = http.createServer((req, res) => {
    // Determine the file path
    let filePath = path.join(PUBLIC_DIR, req.url === '/' ? 'index.html' : req.url);

    // Check if the requested path exists
    fs.exists(filePath, (exists) => {
        if (!exists) {
            // 404 Not Found
            res.writeHead(404, { 'Content-Type': 'text/plain' });
            res.end('404 File Not Found\n');
            return;
        }

        // Check if the path is a directory (shouldn't happen with the logic above, but good practice)
        if (fs.statSync(filePath).isDirectory()) {
            // For a directory, attempt to serve index.html within it
            filePath = path.join(filePath, 'index.html');
        }

        // Read the file asynchronously
        fs.readFile(filePath, (err, data) => {
            if (err) {
                // 500 Internal Server Error (e.g., permission error)
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end(`Server Error: ${err.code}\n`);
            } else {
                // Success: 200 OK
                const contentType = getContentType(filePath);
                res.writeHead(200, { 'Content-Type': contentType });
                res.end(data, 'utf-8');
            }
        });
    });
});

// Start the server
server.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}/`);
    console.log(`Serving files from: ${PUBLIC_DIR}`);
});