Type.registerNamespace("Sys.Extended.UI");Type.registerNamespace("Seadragon");Sys.Extended.UI.Seadragon.Strings={Errors:{Failure:"Sorry, but Seadragon Ajax can't run on your browser!\nPlease try using IE 7 or Firefox 3.\n",Dzc:"Sorry, we don't support Deep Zoom Collections!",Dzi:"Hmm, this doesn't appear to be a valid Deep Zoom Image.",Xml:"Hmm, this doesn't appear to be a valid Deep Zoom Image.",Empty:"You asked us to open nothing, so we did just that.",ImageFormat:"Sorry, we don't support {0}-based Deep Zoom Images.",Security:"It looks like a security restriction stopped us from loading this Deep Zoom Image.",Status:"This space unintentionally left blank ({0} {1}).",Unknown:"Whoops, something inexplicably went wrong. Sorry!"},Messages:{Loading:"Loading..."},Tooltips:{FullPage:"Toggle full page",Home:"Go home",ZoomIn:"Zoom in",ZoomOut:"Zoom out"},getString:function(e){for(var c=e.split("."),a=Sys.Extended.UI.Seadragon.Strings,b=0;b<c.length;b++)a=a[c[b]]||{};if(typeof a!="string")a="";var d=arguments;return a.replace(/\{\d+\}/g,function(b){var a=parseInt(b.match(/\d+/))+1;return a<d.length?d[a]:""})},setString:function(e,d){for(var c=e.split("."),b=Seadragon.Strings,a=0;a<c.length-1;a++){if(!b[c[a]])b[c[a]]={};b=b[c[a]]}b[c[a]]=d}};Seadragon.Strings=Sys.Extended.UI.Seadragon.Strings;