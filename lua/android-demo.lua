function callAndroidApi(context,layout,tip)
	builder = luajava.newInstance("android.app.AlertDialog$Builder", context)
	builder:setMessage(luajava.newInstance("java.lang.String", "lua吗？"))
	builder:setTitle(luajava.newInstance("java.lang.String", "提示"))
	button_cb = {}
	function button_cb.onClick(dialog, which)
		dialog:dismiss();
	end
	buttonProxy = luajava.createProxy("android.content.DialogInterface$OnClickListener",  button_cb)
	builder:setPositiveButton(luajava.newInstance("java.lang.String", "确认"), buttonProxy)
	builder:setNegativeButton(luajava.newInstance("java.lang.String", "取消"), buttonProxy)
	builder:create()
	builder:show()
	
	tv = luajava.newInstance("android.widget.TextView",context)
	strTk = luajava.newInstance("java.util.StringTokenizer", "a,b,c,d", ",")
	tv:setText(strTk:nextToken())
	layout:addView(tv)
	
end
