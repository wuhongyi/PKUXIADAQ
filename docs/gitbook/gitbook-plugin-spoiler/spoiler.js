require(["gitbook", "jquery"], function(gitbook, $) {
	gitbook.events.bind("page.change", function(){

    $('.spoiler').hover(function(){
      $(this).addClass('hover');
    }, function(){
      $(this).removeClass('hover');
    });

	});
});
