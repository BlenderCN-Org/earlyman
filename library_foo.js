mergeInto(LibraryManager.library, {
  my_js: (function() {
      return function() {
        console.log("hello there from the library foo");
        document.getElementById("joe").innerHTML = "changed it";
      };
  }()),
});
