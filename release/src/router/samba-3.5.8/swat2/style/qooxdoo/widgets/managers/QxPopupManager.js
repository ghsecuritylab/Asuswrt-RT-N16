/* Copyright (c): 2002-2005 (Germany): United Internet, 1&1, GMX, Schlund+Partner, Alturo */
function QxPopupManager(){if(QxPopupManager._instance){return QxPopupManager._instance;};QxManager.call(this);QxPopupManager._instance=this;};QxPopupManager.extend(QxManager,"QxPopupManager");proto.update=function(oTarget){var p;for(var vHash in this._objects){p=this._objects[vHash];if(!p.getAutoHide()){continue;};if(!oTarget||p!=oTarget&&(!p.contains(oTarget)||p.getVisible())&&new Date-p.getShowTimeStamp()>100){p.setVisible(false);};};};