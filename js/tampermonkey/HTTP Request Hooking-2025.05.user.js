// ==UserScript==
// @name         HTTP Request Hooking
// @namespace    HTTP
// @version      2025.05
// @description  HTTP Request Hooking
// @author       Vic P.
// @match        https://dev.to/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=vic.onl
// @require      https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js
// @run-at       document-start
// @grant        none
// ==/UserScript==

(function()
{
    "use strict";

    const original_open = XMLHttpRequest.prototype.open;
    const original_send = XMLHttpRequest.prototype.send;

    XMLHttpRequest.prototype.open = function(method, url, async, user, password)
    {
        try {
            let request_info = {
                method,
                url,
                async: async !== undefined ? async : true,
                user: user || null,
                password: password || null,
                timestamp: new Date().toISOString()
            };

            console.log('Request Opened:', request_info);

            // Store request info for later use
            this._request_info = request_info;

            // Call original method
            return original_open.apply(this, arguments);
        } catch (error) {
            console.error('Error in open hook:', error);
            return original_open.apply(this, arguments);
        }
    };

    // Hook into send method
    XMLHttpRequest.prototype.send = function(body)
    {
        try {
            if (body)
            {
                console.log('Request Body:', {
                    body: body,
                    request_info: this._request_info
                });
            }

            this.addEventListener('load', function() {
                try {
                    console.log('Response Received:', {
                        status: this.status,
                        status_text: this.statusText,
                        response_type: this.responseType,
                        response: this.response,
                        request_info: this._request_info
                    });
                } catch (error) {
                    console.error('Error in response hook:', error);
                }
            });

            this.addEventListener('error', function(error) {
                console.error('Request Error:', {
                    error: error,
                    request_info: this._request_info
                });
            });

            return original_send.call(this, body);
        } catch (error) {
            console.error('Error in send hook:', error);
            return original_send.call(this, body);
        }
    };
})();