// ------------------------------------------------
// init.cpp
// ImageProcessor
// ------------------------------------------------

#include "script.h"

// initialize internal dictionaries
void Script::init() {

  Function *m;
  m = new Function;

  // insert built in variables into the variable dictionary
  Item *item = new Item;
  item->type = NUMBER; item->n = 2.178281828459045; v["e"] = *item;
  item->type = NUMBER; item->n = 3.141592653589793; v["pi"] = *item;

  // insert built in operators into the operator dictionary
  m->p = &Script::print; m->args = "item";         o["="] = *m;
  m->p = &Script::add; m->args = "number, number"; o["+"] = *m;
  m->p = &Script::sub; m->args = "number, number"; o["-"] = *m;
  m->p = &Script::mul; m->args = "number, number"; o["*"] = *m;
  m->p = &Script::div; m->args = "number, number"; o["/"] = *m;
  m->p = &Script::mod; m->args = "number, number"; o["%"] = *m;
  m->p = &Script::pwr; m->args = "number, number"; o["^"] = *m;
  m->p = &Script::at; m->args = "list|dict, number"; o["@"] = *m;
  m->p = &Script::nop; m->args = "list start";     o["("] = *m;
  m->p = &Script::makeList; m->args = "list end";  o[")"] = *m;
  m->p = &Script::nop; m->args = "dict start";     o["["] = *m;
  m->p = &Script::makeDict; m->args = "dict end";  o["]"] = *m;
  m->p = &Script::nop; m->args = "proc start";     o["{"] = *m;
  m->p = &Script::makeProc; m->args = "proc end";  o["}"] = *m;

  // insert built in functions into the function dictionary

  // language functions
  m->p = &Script::print; m->args = "item";                 f["print"] = *m;
  m->p = &Script::println; m->args = "item";               f["println"] = *m;
  m->p = &Script::dup;   m->args = "item";                 f["dup"] = *m;
  m->p = &Script::swap;  m->args = "item, item";           f["swap"] = *m;
  m->p = &Script::roll;  m->args = "";                     f["roll"] = *m;
  m->p = &Script::size;  m->args = "";                     f["size"] = *m;
  m->p = &Script::empty; m->args = "";                     f["empty"] = *m;
  m->p = &Script::swap;  m->args = "item, item";           f["swap"] = *m;
  m->p = &Script::def;   m->args = "value, name";          f["def"] = *m;
  m->p = &Script::undef; m->args = "name";                 f["undef"] = *m;
  m->p = &Script::ln;    m->args = "number";               f["ln"] = *m;
  m->p = &Script::neg;   m->args = "number";               f["neg"] = *m;
  m->p = &Script::pop;   m->args = "";                     f["pop"] = *m;
  m->p = &Script::exec;  m->args = "proc";                 f["exec"] = *m;
  m->p = &Script::lt;    m->args = "item, item";           f["lt"] = *m;
  m->p = &Script::eq;    m->args = "item, item";           f["eq"] = *m;
  m->p = &Script::ne;    m->args = "item, item";           f["ne"] = *m;
  m->p = &Script::gt;    m->args = "item, item";           f["gt"] = *m;
  m->p = &Script::doif;  m->args = "bool, proc";           f["if"] = *m;
  m->p = &Script::foreach; m->args = "list|dict, proc";    f["foreach"] = *m;
  m->p = &Script::dofor; m->args = "start step end, proc"; f["for"] = *m;
  m->p = &Script::add;   m->args = "number, number";       f["add"] = *m;
  m->p = &Script::sub;   m->args = "number, number";       f["sub"] = *m;
  m->p = &Script::mul;   m->args = "number, number";       f["mul"] = *m;
  m->p = &Script::div;   m->args = "number, number";       f["div"] = *m;
  m->p = &Script::pwr;   m->args = "number, number";       f["pwr"] = *m;
  m->p = &Script::mod;   m->args = "number, number";       f["mod"] = *m;
  m->p = &Script::list;  m->args = "";                     f["list"] = *m;
  m->p = &Script::dict;  m->args = "";                     f["dict"] = *m;
  m->p = &Script::proc;  m->args = "";                     f["proc"] = *m;
  m->p = &Script::load;  m->args = "list|dict";            f["load"] = *m;
  m->p = &Script::makeList; m->args = "( ...";             f["makeList"] = *m;
  m->p = &Script::makeDict; m->args = "[ ...";             f["makeDict"] = *m;
  m->p = &Script::makeProc; m->args = "{ ...";             f["makeProc"] = *m;
  m->p = &Script::dump;  m->args = "<stack|function|variable|operator>"; f["dump"] = *m;
  m->p = &Script::clear; m->args = "<stack|variable>";     f["clear"] = *m;

  // -------------------------------------
  // image functions
  // -------------------------------------

  // basic I/O
  m->p = &Script::imgRead;    m->args = "filename";        f["imgRead"] = *m;
  m->p = &Script::imgWrite;   m->args = "filename, image"; f["imgWrite"] = *m;
  m->p = &Script::imgDiscard; m->args = "filename, image"; f["imgDiscard"] = *m;
  m->p = &Script::imgShow;    m->args = "image";           f["imgShow"] = *m;
  m->p = &Script::imgUpdate;  m->args = "image";           f["imgUpdate"] = *m;

  // info
  m->p = &Script::imgWidth;   m->args = "image";           f["imgWidth"] = *m;
  m->p = &Script::imgHeight;  m->args = "image";           f["imgHeight"] = *m;

  // morphology
  m->p = &Script::imgInvert;  m->args = "image";           f["imgInvert"] = *m;
  m->p = &Script::imgThreshold; m->args = "level, image";  f["imgThreshold"] = *m;
  m->p = &Script::imgAutoThresholdOtsu; m->args = "image"; f["imgAutoThresholdOtsu"] = *m;
  m->p = &Script::imgIntensity; m->args = "image";         f["imgIntensity"] = *m;
  m->p = &Script::imgErode;   m->args = "image";           f["imgErode"] = *m;
  m->p = &Script::imgDilate;  m->args = "image";           f["imgDilate"] = *m;

  // filter
  m->p = &Script::imgLaplacian;  m->args = "image";        f["imgLaplacian"] = *m;
  m->p = &Script::imgMedian;  m->args = "image";           f["imgMedian"] = *m;
  m->p = &Script::imgSmooth;  m->args = "image";           f["imgSmooth"] = *m;
  m->p = &Script::imgSobel;   m->args = "image";           f["imgSobel"] = *m;
  m->p = &Script::imgConvolve; m->args = "kernel";         f["imgConvolve"] = *m;

  // components
  m->p = &Script::imgFindShapes; m->args = "image";        f["imgFindShapes"] = *m;

  // math
  m->p = &Script::imgAND;     m->args = "image";           f["imgAND"] = *m;
  /*
  m->p = &Script::imgOR;      m->args = "image";           f["imgOR"] = *m;
  m->p = &Script::imgXOR;     m->args = "image";           f["imgXOR"] = *m;
  m->p = &Script::imgDIF;     m->args = "image";           f["imgDIF"] = *m;
  m->p = &Script::imgMAX;     m->args = "image";           f["imgMAX"] = *m;
  m->p = &Script::imgMIN;     m->args = "image";           f["imgMIN"] = *m;
  */
  // ImageMagick functions
  /*
  m->p = &Script::blur; m->args = "radius, sigma, image"; f["imgBlur"] = *m;
  m->p = &Script::crop; m->args = "width, height, offsetx, offsety, image"; f["imgCrop"] = *m;
  m->p = &Script::convolve; m->args = "list, image"; f["imgConvolve"] = *m;
  m->p = &Script::despeckle; m->args = "image"; f["imgDespeckle"] = *m;
  m->p = &Script::edge; m->args = "radius, image"; f["imgEdge"] = *m;
  m->p = &Script::equalize; m->args = "image"; f["imgEqualize"] = *m;
  m->p = &Script::flip; m->args = "image"; f["imgFlip"] = *m;
  m->p = &Script::flop; m->args = "image"; f["imgFlop"] = *m;
  m->p = &Script::negate; m->args = "image"; f["imgNegate"] = *m;
  m->p = &Script::normalize; m->args = "image"; f["imgNormalize"] = *m;
  m->p = &Script::quantize; m->args = "colors, image"; f["imgQuantize"] = *m;
  m->p = &Script::imgroll; m->args = "columns, rows, image"; f["imgRoll"] = *m;
  m->p = &Script::rotate; m->args = "degrees, image"; f["imgRotate"] = *m;
  m->p = &Script::sample; m->args = "width, height, image"; f["imgSample"] = *m;
  m->p = &Script::scale; m->args = "width, height, image"; f["imgScale"] = *m;
  m->p = &Script::sharpen; m->args = "radius, sigma, image"; f["imgSharpen"] = *m;
  m->p = &Script::shear; m->args = "xangle, yangle, image"; f["imgShear"] = *m;
  m->p = &Script::threshold; m->args = "value, image"; f["imgThreshold"] = *m;
  m->p = &Script::zoom; m->args = "width, height, image"; f["imgZoom"] = *m;

  m->p = &Script::imgadd; m->args = "image1, image2"; f["imgAdd"] = *m;
  */
}
