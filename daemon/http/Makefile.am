BUILT_SOURCES = index_html.h bootstrap_min_css.h metisMenu_min_css.h sb-admin-2_css.h \
	bootstrap_min_js.h metisMenu_min_js.h sb-admin-2_js.h menu_html.h jquery_min_js.h \
	header_page_html.h footer_page_html.h

index_html : index.html
	cp $< $@

index_html.h : index_html
	xxd -i $< $@

bootstrap_min_css : vendor/bootstrap/css/bootstrap.min.css
	cp $< $@

bootstrap_min_css.h : bootstrap_min_css
	xxd -i $< $@

metisMenu_min_css : vendor/metisMenu/metisMenu.min.css
	cp $< $@

metisMenu_min_css.h : metisMenu_min_css
	xxd -i $< $@

sb-admin-2_css : dist/css/sb-admin-2.css
	cp $< $@

sb-admin-2_css.h : sb-admin-2_css
	xxd -i $< $@

bootstrap_min_js : vendor/bootstrap/js/bootstrap.min.js
	cp $< $@

bootstrap_min_js.h : bootstrap_min_js
	xxd -i $< $@

metisMenu_min_js : vendor/metisMenu/metisMenu.min.js
	cp $< $@

metisMenu_min_js.h : metisMenu_min_js
	xxd -i $< $@

sb-admin-2_js : dist/js/sb-admin-2.js
	cp $< $@

sb-admin-2_js.h : sb-admin-2_js
	xxd -i $< $@

menu_html : menu.html
	cp $< $@

menu_html.h : menu_html
	xxd -i $< $@

jquery_min_js : vendor/jquery/jquery.min.js
	cp $< $@

jquery_min_js.h : jquery_min_js
	xxd -i $< $@

# bootstrap_min_css_map : vendor/bootstrap/css/bootstrap.min.css.map
# 	cp $< $@

# bootstrap_min_css_map.h : bootstrap_min_css_map
# 	xxd -i $< $@

header_page_html : header_page.html
	cp $< $@

header_page_html.h : header_page_html
	xxd -i $< $@

footer_page_html : footer_page.html
	cp $< $@

footer_page_html.h : footer_page_html
	xxd -i $< $@

clean-local:
	rm *_html *_js *_css $(BUILT_SOURCES)
