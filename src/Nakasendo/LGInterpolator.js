var binary = require('node-pre-gyp');//
var path = require('path')
var binding_path = binary.find(path.resolve(path.join(__dirname,'../../package.json')));
var nakesendoBindings = require(binding_path)


class LGInterpolator{
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
        return nakesendoBindings.LGInterpolatorFull(this.#m_xfx, xval, this.#m_modulo, this.#m_IsDecimal);
    }
    
    
    evaluateBasis(xval, basisPt){
        return nakesendoBindings.LGInterpolatorSingle(this.#m_xfx, xval, this.#m_modulo, this.#m_IsDecimal, basisPt);
    }

}

module.exports = {
    LGInterpolator
 }
