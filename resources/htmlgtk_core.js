
function  htmlgtk_js_object(obj,getter,setter) {
  this_obj = { addr: obj };
  this_handler = {
    get: getter,
    set: setter
  };

//  print('obj:'+obj+' get:'+getter+' set:'+setter);

  return new Proxy(this_obj,this_handler);
};

function  htmlgtk_js_object_get_addr(obj) {
  return obj.addr;
};
