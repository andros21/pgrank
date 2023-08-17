// code.js
// =======
// Reference:
//  * https://github.com/cytoscape/cytoscape.js/tree/unstable/documentation/demos/cose-layout
//  * https://github.com/cytoscape/cytoscape.js-popper/blob/master/demo-tippy.html

Promise.all([
  fetch("cy-style.json", { mode: "no-cors" }).then(function (res) {
    return res.json();
  }),
  fetch("data.json", { mode: "no-cors" }).then(function (res) {
    return res.json();
  }),
]).then(function (dataArray) {
  var cy = (window.cy = cytoscape({
    container: document.getElementById("cy"),

    layout: {
      name: "cose",
      idealEdgeLength: 100,
      nodeOverlap: 20,
      refresh: 20,
      fit: true,
      padding: 30,
      randomize: false,
      componentSpacing: 100,
      nodeRepulsion: 400000,
      edgeElasticity: 100,
      nestingFactor: 5,
      gravity: 80,
      numIter: 1000,
      initialTemp: 200,
      coolingFactor: 0.95,
      minTemp: 1.0,
    },

    style: dataArray[0],

    elements: dataArray[1]["elements"],
  }));
  var makeTippy = function (node) {
    var ref = node.popperRef();
    // Since tippy constructor requires DOM element/elements, create a placeholder
    var dummyDomEle = document.createElement("div");
    var tip = tippy(dummyDomEle, {
      getReferenceClientRect: ref.getBoundingClientRect,
      trigger: "manual", // mandatory
      // dom element inside the tippy:
      content: function () {
        // function can be better for performance
        var div = document.createElement("div");
        div.innerHTML = `
             <strong>id:</strong> <a href='${node.data("url")}'>${node.data(
               "name",
             )}</a><br>
             <strong>rank:</strong> ${node.data("rank")}<br>
             <strong>name:</strong> ${node.data("full_name")}<br>
             <strong>zone:</strong> ${node.data("zone")}<br>`;
        return div;
      },
      // your own preferences:
      arrow: true,
      placement: "bottom",
      sticky: "reference",
      // if interactive:
      interactive: true,
      appendTo: document.body, // or append dummyDomEle to document.body
    });
    return tip;
  };
  cy.on("tap", "node", function (event) {
    var node = event.target;
    var tippy = makeTippy(node);
    tippy.show();
  });
});
