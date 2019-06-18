# resume-generator
A simple but extensible resume generator that turns XML or (hopefully soon) YAML into HTML.

## Usage
```
resume my_resume.xml [output (optional)]
```

## Features
 * Use your own stylesheets
 * Automatic parsing of URLs to HTML `<a>` tags
 * Define your own XML tags using the Mustache templating engine
 
 ```xml
     <Header Optional="Address; Email; LinkedIn; Website">
      <header>
        <h1 id="name">Vincent La</h1>
        <p>
          {{#Address}}<span>{{Value}}</span>{{/Address}}
          {{#Email}}<span>{{{Value}}}</span>{{/Email}}
          {{#LinkedIn}}<span>{{{Value}}}</span>{{/LinkedIn}}
          {{#Website}}<span>{{{Value}}}</span>{{/Website}}
        </p>
      </header>
    </Header>
```

### Planned Features
 * Command line based help system in case you forget what XML tags you can use and what their attributes are
 * Ability to "subclass" custom XML tags
 * Ability to create different resumes from the same XML data file
 * More user-friendly error messages
