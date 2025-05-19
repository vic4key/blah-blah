// ==UserScript==
// @name         Swagger UI Enhancement
// @namespace    http://tampermonkey.net/
// @version      1.1
// @description  Swagger UI Enhancement
// @author       Vic P.
// @match        http://localhost:*/docs*
// @match        https://petstore.swagger.io*
// @require      https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js
// @run-at       document-start
// @grant        none
// ==/UserScript==

let log = (...args) => console.log(`%cSwagger >`, 'font-weight: bold; color: red;', ...args)
let log_error = (...args) => console.error(`%cSwagger >`, 'font-weight: bold; color: red;', ...args)

const toggle_button_title_show = "Show Examples ▼";
const toggle_button_title_hide = "Hide Examples ▲";

function create_toggle_button(opblock_section_name, opblock_section_header) {
    let class_name = `${opblock_section_name}-toggle-button`;
    let toggle_button = opblock_section_header.find(class_name);
    if (toggle_button?.length == 0) {
        toggle_button = $('<button>')
            .text(toggle_button_title_show)
            .addClass("btn")
            .addClass(class_name.slice(1))
            .css({ "margin-left": "5px" });
        opblock_section_header.append(toggle_button);
    }
    return toggle_button;
}

function update_ui_for_opblock_section(opblock_section_description, opblock_section_content_name_list, callback) {
    let opblock_section_content_name = null;
    opblock_section_content_name_list.forEach(name => {
        let temp = opblock_section_description.find(name);
        if (temp?.length != 0) {
            opblock_section_content_name = name;
            return;
        }
    });

    let opblock_section_content = opblock_section_description.find(opblock_section_content_name);
    if (opblock_section_content?.length == 0) {
        log("opblock_section_content not found");
        return;
    }

    if (callback) {
        callback(opblock_section_content.last());
    }
}

function setup_toggle_in_opblock_section_impl(opblock, opblock_section_name, opblock_section_description_name, opblock_section_content_name_list) {
    let button = opblock.find(".opblock-summary-control");
    if (button?.length == 0) {
        log_error(button.text(), "opblock_section button not found");
        return false;
    }

    let opblock_section = opblock.find(opblock_section_name);
    if (opblock_section?.length == 0) {
        log_error(button.text(), "opblock_section not found");
        return false;
    }

    let opblock_section_header = opblock_section.find(".opblock-section-header");
    let opblock_section_description = opblock_section.find(opblock_section_description_name);
    if (opblock_section_header?.length == 0 || opblock_section_description?.length == 0) {
        log_error(button.text(), "opblock_section header or .opblock_section body not found");
        return false;
    }

    let toggle_button = create_toggle_button(opblock_section_name, opblock_section_header);
    toggle_button.unbind('click').on('click', function() {
        update_ui_for_opblock_section(opblock_section_description, opblock_section_content_name_list, opblock_section_content => {
            let opblock_section_content_hidden = opblock_section_content.is(':hidden');
            if (opblock_section_content_hidden) {
                opblock_section_content.show();
                toggle_button.text(toggle_button_title_hide);
            }
            else {
                opblock_section_content.hide();
                toggle_button.text(toggle_button_title_show);
            }
        });
    });

    update_ui_for_opblock_section(opblock_section_description, opblock_section_content_name_list, opblock_section_content => {
        let try_it_out_enabled = opblock_section_description?.find("textarea")?.length != 0;
        if (try_it_out_enabled) {
            opblock_section_content.show(); // always shown when "try it out" enabled
            toggle_button.text(toggle_button_title_hide);
        }
        else {
            opblock_section_content.hide(); // auto hidden as default
            toggle_button.text(toggle_button_title_show);
        }
        // toggle_button.text(opblock_section_content.is(':hidden') ? toggle_button_title_show : toggle_button_title_hide);
    });

    let api_name = button.find(".opblock-summary-method")?.text() + " " + button.find(".opblock-summary-path")?.attr("data-path");
    log(`${api_name} => OK`);

    return true;
}

function setup_toggle_in_opblock_section(opblock, delay_ms = 0) {
    if (opblock.hasClass("opblock") && opblock.hasClass("is-open")) {
        setTimeout(function () {
            setup_toggle_in_opblock_section_impl(opblock, ".responses-wrapper", ".responses-inner", [".responses-table"]); // response section
            setup_toggle_in_opblock_section_impl(opblock, ".opblock-section-request-body", ".opblock-description-wrapper", [".model-example", ".body-param"]); // body section
        }, delay_ms);
    }
}

function setup_trigger_event_when_user_click_in_opblock_section() {
    let observer = new MutationObserver((mutations) => {
        mutations.forEach((mutation) => {
            if (mutation.addedNodes.length) {
                let opblock = $(mutation.addedNodes[0].parentNode);
                setup_toggle_in_opblock_section(opblock, 250);

                $(".try-out__btn").unbind('click').on('click', function(e) {
                    let opblock = $(this).closest(".opblock");
                    setup_toggle_in_opblock_section(opblock);
                });
            }
        });
    });
    observer.observe(document.body, {
        childList: true,
        subtree: true
    });
}

function setup_initializing() {
    $('.opblock').each(function(i, _) {
        let opblock = $(_);
        setup_toggle_in_opblock_section(opblock);
    });

    $(".try-out__btn").unbind('click').on('click', function(e) {
        let opblock = $(this).closest(".opblock");
        setup_toggle_in_opblock_section(opblock);
    });
}

document.addEventListener('DOMContentLoaded', function() {
    setTimeout(function () {
        setup_initializing();
        setup_trigger_event_when_user_click_in_opblock_section();
    }, 1000);
});