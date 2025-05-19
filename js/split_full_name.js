/**
 * Splits a full name into first, middle, and last name components
 * @param {string} full_name - The full name to split
 * @param {Object} options - Configuration options
 * @param {Array<string>} [options.titles=['Mr', 'Mrs', 'Ms', 'Dr', 'Prof', 'Sir', 'Lady', 'Lord']] - Titles to recognize
 * @param {Array<string>} [options.suffixes=['Jr', 'Sr', 'II', 'III', 'IV', 'PhD', 'MD', 'DDS']] - Suffixes to recognize
 * @param {boolean} [options.handle_compound_last_names=false] - Whether to treat last two parts as compound last name for Spanish/Portuguese names
 * @returns {Object} An object containing first_name, middle_name, last_name, title, and suffix
 */
function split_full_name(full_name, options = {}) 
{
    // Default options
    const default_options = 
    {
        titles: ['Mr', 'Mrs', 'Ms', 'Dr', 'Prof', 'Sir', 'Lady', 'Lord'],
        suffixes: ['Jr', 'Sr', 'II', 'III', 'IV', 'PhD', 'MD', 'DDS'],
        handle_compound_last_names: false
    };
    
    const config = { ...default_options, ...options };
    
    // Handle empty or null input
    if (!full_name || typeof full_name !== 'string') 
    {
        return { title: '', first_name: '', middle_name: '', last_name: '', suffix: '' };
    }

    // Trim and normalize whitespace
    const normalized_name = full_name.trim().replace(/\s+/g, ' ');
    
    // Split the name into parts
    let name_parts = normalized_name.split(' ');
    
    // Check for title
    let title = '';
    if (name_parts.length > 1 && config.titles.includes(name_parts[0])) 
    {
        title = name_parts[0];
        name_parts = name_parts.slice(1);
    }
    
    // Check for suffix
    let suffix = '';
    if (name_parts.length > 1 && config.suffixes.includes(name_parts[name_parts.length - 1])) 
    {
        suffix = name_parts[name_parts.length - 1];
        name_parts = name_parts.slice(0, name_parts.length - 1);
    }
    
    // Handle different cases based on number of parts
    if (name_parts.length === 0) 
    {
        return { title, first_name: '', middle_name: '', last_name: '', suffix };
    } 
    else if (name_parts.length === 1) 
    {
        // Only first name
        return { title, first_name: name_parts[0], middle_name: '', last_name: '', suffix };
    } 
    else if (name_parts.length === 2) 
    {
        // First and last name
        return { title, first_name: name_parts[0], middle_name: '', last_name: name_parts[1], suffix };
    } 
    else 
    {
        // Handle compound last names for Spanish/Portuguese names if option is set
        let first_name, middle_name, last_name;
        
        first_name = name_parts[0];
        
        if (config.handle_compound_last_names && name_parts.length >= 3) 
        {
            // Treat the last two parts as a compound last name (e.g., "Garcia Lopez")
            last_name = name_parts.slice(name_parts.length - 2).join(' ');
            middle_name = name_parts.length > 3 ? name_parts.slice(1, name_parts.length - 2).join(' ') : '';
        } 
        else 
        {
            // Standard processing: last part is the last name
            last_name = name_parts[name_parts.length - 1];
            middle_name = name_parts.slice(1, name_parts.length - 1).join(' ');
        }
        
        return { title, first_name, middle_name, last_name, suffix };
    }
}

// Test cases
[
    'John Doe',
    'Mary Jane Smith',
    'James Robert John Williams',
    'Sarah',
    'José María García',
    'Jean-Claude Van Damme',
    'Martin Luther King Jr',
    'Sir Arthur Conan Doyle',
    '',
    '  William  Jefferson  Clinton  ',
    null,
    'Dr Jane Goodall',
    'John Smith Jr',
    'Mr Robert John Smith III',
    'John O\'Connor',
    'Anna-Maria Rodriguez',
    'Juan Carlos Garcia Lopez',
].forEach(name => {
    const result = split_full_name(name);
    console.log(`Input: "${name === null ? 'null' : name}"`);
    console.log(`Result: ${JSON.stringify(result, null, 2)}\n`);
});

// Test with compound last name handling for specific cases
console.log('\nWith Compound Last Name Handling:');
['Juan Carlos Garcia Lopez', 'Maria del Carmen Vega Ortiz'].forEach(name => {
    const result = split_full_name(name, { handleCompoundLastNames: true });
    console.log(`Input: "${name}"`);
    console.log(`Result: ${JSON.stringify(result, null, 2)}\n`);
});