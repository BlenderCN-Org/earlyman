mergeInto(LibraryManager.library, {
  my_js: (function() {
      return function(content) {
        console.log("hello there from the library foo");
        document.getElementById("joe").innerHTML = content;
      };
  }()),
});
