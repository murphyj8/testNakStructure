var binary = require('node-pre-gyp');//
var path = require('path')
console.log(__dirname)
var binding_path = binary.find(path.resolve(path.join(__dirname,'../../package.json')));
console.log(binding_path);
var nakesendoBindings = require(binding_path)


class ECLGInterpolator{
    #m_xfx = [];
    #m_IsDecimal=false;
    #m_modulo;
    
    
    constructor(xfx, modulo, IsDecimal=false){
        this.#m_xfx = xfx;
        this.#m_modulo = modulo;
        this.#m_IsDecimal = IsDecimal;      
    }
    
    get xfx(){
        return this.#m_xfx;
    }
    
    get mod(){
        return this.#m_modulo;
    }
    
    get isdec(){
        return this.#m_IsDecimal;
    }
    
    evaluate(xval){
        return nakesendoBindings.ECLGInterpolatorFull(this.#m_xfx, xval, this.#m_modulo, this.#m_IsDecimal);
    }
    
    
    
}

module.exports = {
    ECLGInterpolator
 }
